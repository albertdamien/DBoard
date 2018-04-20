/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  LED bar library V2.0
  Copyright (c) 2010 Seeed Technology Inc.

  Original Author: LG

  Modify: Damien ALBERT, 2018-04-18
  Intregration to d|board framework

  Modify: Loovee, 2014-2-26
  User can choose which Io to be used.
  
  Modify: Long, 2015-01-07
  User can change the brightness level for each LED segment
  Rename constant to avoid name conflict
  
  The MIT License (MIT)

  Copyright (c) 2013 Seeed Technology Inc.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <LedBar.h>

// avoid name conflict
#define GLB_CMDMODE 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data

LedBar::LedBar(int _dataPin, int _clockPin, bool _greenToRed) :
    m_dataPin(_dataPin),
    m_clockPin(_clockPin),
    m_greenToRed(_greenToRed) {
    for (byte i = 0; i < 10; i++)
        m_state[i] = 0x00;  // persist state so individual leds can be toggled
} // constructor

LedBar::~LedBar() {
    digitalWrite(m_clockPin, LOW);
    digitalWrite(m_dataPin, LOW);
} // destructor

void LedBar::setup() {
    pinMode(m_clockPin, OUTPUT);
    pinMode(m_dataPin, OUTPUT);
} // setup

void LedBar::run(byte _serv) {
    ; // nothing here
} // run

// change the orientation / green to red, or red to green
void LedBar::setGreenToRed(bool _greenToRed) {
    m_greenToRed = _greenToRed;
    setData(m_state);
} // setGreenToRed

// set level (0-10)
// level 0 means all leds off
// level 10 means all leds on
// level 4.5 means 4 LEDs on and the 5th LED's half on
void LedBar::setLevel(float _level) {
    _level = max(0, min(10, _level));
    _level *= 8; // there are 8 (noticable) levels of brightness on each segment
    // Place number of '_level' of 1-bits on m_state
    for (byte i = 0; i < 10; i++) {
        m_state[i] = (_level > 8) ? ~0 : (_level > 0) ? ~(~0 << byte(_level)) : 0;
        _level -= 8;
    }
    setData(m_state);
} // setLevel

// set a single led
// led (1-10)
// brightness (0-1)
void LedBar::setLed(byte _led, float _brightness) {
    _led = max(1, min(10, _led));
    _brightness = max(0, min(_brightness, 1));

    // Zero based index 0-9 for bitwise operations
    _led--;

    // 8 (noticable) levels of brightness
    // 00000000 darkest
    // 00000011 brighter
    // ........
    // 11111111 brightest
    m_state[_led] = ~(~0 << (byte) (_brightness * 8));

    setData(m_state);
} // setLed

// toggle a single led
// led (1-10)
void LedBar::toggleLed(byte _led) {
    _led = max(1, min(10, _led));

    // Zero based index 0-9 for bitwise operations
    _led--;

    m_state[_led] = m_state[_led] ? 0 : ~0;

    setData(m_state);
} // toggleLed

void LedBar::setBits(unsigned int _bits) {

    for (byte i = 0; i < 10; i++) {
        if ((_bits % 2) == 1)
            m_state[i] = 0xFF;
        else
            m_state[i] = 0x00;
        _bits /= 2;
    }

    setData(m_state);
} // setBits

// Return the current bits
unsigned int const LedBar::getBits() {
    unsigned int _bits = 0x00;
    for (byte i = 0; i < 10; i++) {
        if (m_state[i] != 0x0) _bits |= (0x1 << i);
    }
    return _bits;
} // getBits

// send 16 bits of data
void LedBar::sendData(unsigned int _data) {
    unsigned int state = 0;
    for (byte i = 0; i < 16; i++) {
        unsigned int state1 = (_data & 0x8000) ? HIGH : LOW;
        digitalWrite(m_dataPin, state1);

        state = 1 - state;
        digitalWrite(m_clockPin, state);

        _data <<= 1;
    }
} // sendData

// send the latch command
void LedBar::latchData() {
  digitalWrite(m_dataPin, LOW);
  delayMicroseconds(10);

  for (byte i = 0; i < 4; i++) {
    digitalWrite(m_dataPin, HIGH);
    digitalWrite(m_dataPin, LOW);
  }
} // latchData

// each element in the state will hold the brightness level
// 00000000 darkest
// 00000011 brighter
// ........
// 11111111 brightest
void LedBar::setData(byte _state[]) {

    sendData(GLB_CMDMODE);

    for (byte i = 0; i < 10; i++) {
        if (m_greenToRed) {
            // Go backward on _state
            sendData(_state[10-i-1]);
        } else {
            // Go forward on _state
            sendData(_state[i]);
        }
    }
    // Two extra empty bits for padding the command to the correct length
    sendData(0x00);
    sendData(0x00);

    latchData();
} // setData
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

#ifndef _LED_BAR_H_
#define _LED_BAR_H_

#include <Arduino.h>
#include <Component.h>

class LedBar : public Component {
    public:
        LedBar(int dataPin, int clockPin, bool greenToRed);
        ~LedBar();
        void setup();
        void run(byte);
        void setGreenToRed(bool greenToRed);       // (re)set orientation
        void setLevel(float level);                // set level, range from 0 to 10
        void setLed(byte led, float brightness);   // set brightness for a single led, range from 0 to 1
        void toggleLed(byte led);                  // toggle a single led
        void setBits(unsigned int bits);           // toggle leds to match given bits
        unsigned int const getBits();              // get the current state 
        
    protected:
        void sendData(unsigned int data);   // send a word to led bar
        void latchData();                   // load data into the latch register
        void setData(byte bits[]);          // set data array
        int m_clockPin;                     // clock pin
        int m_dataPin;                      // data pin
        bool m_greenToRed;                  // orientation (0 = red to green, 1 = green to red)
        byte m_state[10];                   // led state, brightness for each LED
};

#endif // _LED_BAR_H_
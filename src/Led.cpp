/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <Led.h>

Led::Led(int _pin) :
    m_ledPin(_pin),
    m_ledState(LOW) {
    // nothing left to write here
} // constructor

Led::~Led() {
    digitalWrite(m_ledPin, LOW);
} // destructor

void Led::setup() {
    pinMode(m_ledPin, OUTPUT);
    digitalWrite(m_ledPin, LOW);
} // setup

void Led::run(byte _serv) {
    ; // nothing here
} // run

void Led::on() {
    leave();
    setOn();
} // on

void Led::dim(int _val) {
    leave();
    setDim(_val);
} // dim

void Led::off() {
    leave();
    setOff();
} // off

void Led::toggle() {
    leave();
    setToggle();
} // toggle

void Led::setOn() {
    m_ledState = HIGH;                  // turn it on
    digitalWrite(m_ledPin, m_ledState); // update the actual LED
} // setOn

void Led::setDim(int _val) {
    m_ledState = HIGH;                  // turn it on
    analogWrite(m_ledPin, _val);        // update the actual PWM LED value
} // setDim

void Led::setOff() {
    m_ledState = LOW;                   // turn it off
    digitalWrite(m_ledPin, m_ledState); // update the actual LED
} // setOff

void Led::setToggle() {
    m_ledState = (m_ledState == LOW) ? HIGH : LOW;
    digitalWrite(m_ledPin, m_ledState);
} // setToggle
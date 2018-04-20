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

#include <Input.h>

Input::Input(int _pin, bool _poolup) :
  m_inputPin(_pin),
  m_pullup(_poolup) {
  // si _pin est digital alors pinMode sinon rien
} // constructor

void Input::setup() {
    pinMode(m_inputPin, (m_pullup)? INPUT_PULLUP : INPUT);
    wait(0);
} // setup

void Input::run(serviceType _serv) {
    ; // nothing here
} // run

int Input::read() {
  if (m_inputPin > 13 && m_inputPin < 20) 
    return analogRead(m_inputPin); // analog pin
	else 
    return digitalRead(m_inputPin); // digital pin
} // read
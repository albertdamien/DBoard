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

#include <RotaryAngleSensor.h>

RotaryAngleSensor::RotaryAngleSensor(int _pin) : 
    Input(_pin, false),
    m_value(read()),
    m_threshold(2) {
    // nothing more
} // constructor

void RotaryAngleSensor::run(serviceType _serv) {
    if (abs(m_value - read()) > m_threshold) event(EVENT_KNOB);
    m_value = read();
    wait(100);
} // run

void RotaryAngleSensor::setup() {
    Input::setup();
    m_value = read();
} // setup

int RotaryAngleSensor::value() {
    m_value = read();
    return m_value;
} // value

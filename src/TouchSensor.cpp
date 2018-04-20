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

#include <TouchSensor.h>

TouchSensor::TouchSensor(int _pin) : 
    Input(_pin, true),
    m_state(0) {
    // nothing more
} // constructor

void TouchSensor::run(serviceType _serv) {
    int prevState = m_state;
    m_state = read();
    if (prevState == LOW && m_state == HIGH) {
        m_touchMillis = millis();
    }
    else if (prevState == HIGH && m_state == LOW) {
        if (millis() - m_touchMillis < 50) {
            // ignore this for debounce
        } else if (millis() - m_touchMillis < 300) {
            event(EVENT_TOUCH_SHORT);
        } else {
            event(EVENT_TOUCH_LONG);
        }
    }
    wait(10);
} // run
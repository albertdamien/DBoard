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

#include <ButtonSensor.h>

ButtonSensor::ButtonSensor(int _pin) : 
    Input(_pin, true),
    m_state(0) {
    // nothing more
} // constructor

void ButtonSensor::run(serviceType _serv) {
    int prevState = m_state;
    m_state = read();
    if (prevState == LOW && m_state == HIGH) {
        m_pressedMillis = millis();
    }
    else if (prevState == HIGH && m_state == LOW) {
        if (millis() - m_pressedMillis < 50) {
            // ignore this for debounce
        } else if (millis() - m_pressedMillis < 300) {
            event(EVENT_BTN_SHORT_PRESS);
        } else {
            event(EVENT_BTN_LONG_PRESS);
        }
    }
    wait(10);
} // run
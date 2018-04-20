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

#include <PIRMotionSensor.h>

PIRMotionSensor::PIRMotionSensor(int _pin) : 
    Input(_pin, false),
    m_moving(false) {
    // nothing else to write here
} // constructor

void PIRMotionSensor::run(serviceType _serv) {
    if (digitalRead(m_inputPin) == HIGH) {
        if (!m_moving) event(EVENT_PIR_DETECT);
        event(EVENT_PIR_MOVING);
        m_moving = true;
        wait(1000);
    } else {
        m_moving = false;
        wait(50);
     }
} // run
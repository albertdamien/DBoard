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

#include <LightSensor.h>

LightSensor::LightSensor(int _pin) : 
    Input(_pin, false),
    m_thresholdDayLight(400),
    m_value(0),
    m_day(false),
    m_changeMillis(0) {
    // nothing else to write here
} // constructor

void LightSensor::setup() {
    Input::setup();
    m_value = read();
    if ((m_value > m_thresholdDayLight) && !m_day) {
        m_day = true;
    }
    if ((m_value < m_thresholdDayLight) && m_day) {
        m_day = false;
    }
} // setup

void LightSensor::run(serviceType _serv) {
    int prevValue = m_value;
    m_value = read();
    if (abs(m_value - prevValue) > 2) event(EVENT_LUX_CHANGE);
    if ((m_value > m_thresholdDayLight) && (prevValue < m_thresholdDayLight) && !m_day) {
        m_changeMillis = millis();
    }
    if ((m_value < m_thresholdDayLight) && (prevValue > m_thresholdDayLight) && m_day) {
        m_changeMillis = millis();
    }
    if ((millis() - m_changeMillis) > 2000) {
        if ((m_value > m_thresholdDayLight) && !m_day) {
            m_day = true;
            event(EVENT_LUX_DAY);
        }
        if ((m_value < m_thresholdDayLight) && m_day) {
            m_day = false;
            event(EVENT_LUX_NIGHT);
        }
        m_changeMillis = millis();
    }
    wait(100);
} // run

int LightSensor::value() {
    m_value = read();
    return map(constrain(m_value, 0, 782), 0, 782, 0, 100); // map to %;
} // value
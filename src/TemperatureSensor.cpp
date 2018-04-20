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

#include <TemperatureSensor.h>

TemperatureSensor::TemperatureSensor(int _pin) : 
    Input(_pin, false),
    m_hotThreshold(30),
    m_coldThreshold(15),
    m_value(0) {
    // nothing more
} // constructor

void TemperatureSensor::run(serviceType _serv) {
    float prev = m_value;
    int val = read();                  
    m_value=1/(log((float)(1023-val)/val)/3975+1/298.15)-273.15;
    if (m_value > m_hotThreshold) event(EVENT_TEMP_HOT);
    if (m_value < m_coldThreshold) event(EVENT_TEMP_COLD);
    if (abs(m_value - prev) > 0.1) event(EVENT_TEMP_CHANGE);
    wait(1000);
} // run

float TemperatureSensor::value() {
    return m_value;
} // value

void TemperatureSensor::tune(float _hot, float _cold) {
    m_hotThreshold = _hot;
    m_coldThreshold = _cold;
} // tune

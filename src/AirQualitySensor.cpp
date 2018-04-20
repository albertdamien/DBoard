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

#include <AirQualitySensor.h>

#define STATE_INIT 0
#define STATE_READ 1

AirQualitySensor::AirQualitySensor(int _pin) :
    Input(_pin, false),
    m_value(read()),
    m_state(STATE_INIT),
    m_init_retry(0),
    m_avg_index(0),
    m_avg_total(0) {
    // nothing left to write here
} // constructor

void AirQualitySensor::run(byte _serv) {
    int last_value = m_value;
    m_value = read();
    switch(m_state) {
        case STATE_INIT:
            if (m_value < 798 && m_value > 10) { // the init voltage is ok
                m_avg_value = m_value;
                event(EVENT_AIR_READY);
                m_state = STATE_READ;
                wait(0);
                break;
            } else if(m_value > 798 || m_value <= 10) {   
                if(m_init_retry++ == 5) {
                    event(EVENT_AIR_ERROR);
                } else {
                    event(EVENT_AIR_WARMUP);
                    wait(60000); // 1min 
                }
            }
            break;
        case STATE_READ:
            // compute average value
            if (m_avg_index == 150) { // sum 5 minutes
                m_avg_value = m_avg_total / 150; 
                m_avg_total = 0;
                m_avg_index = 0;        
            } else {
                m_avg_total+=m_value;
                m_avg_index++;
            }
            // compute slope & send event
            if ((m_value - last_value > 400) || (m_value > 700))
                event(EVENT_AIR_POLLUTION_HIGH);
            else if (((m_value-last_value > 400) && (m_value < 700)) || (m_value - m_avg_value > 150))
                event(EVENT_AIR_POLLUTION_MEDIUM);
            else if(((m_value - last_value > 200) && (m_value < 700)) || (m_value - m_avg_value > 50))
                event(EVENT_AIR_POLLUTION_LOW);
            else
                event(EVENT_AIR_FRESH_AIR);
            // wait next 2s
            wait(2000);
            break;
    }
} // run

int AirQualitySensor::value() {
    return m_value;
} // value

bool AirQualitySensor::isReady() {
    return (m_state == STATE_READ);
} // isReady
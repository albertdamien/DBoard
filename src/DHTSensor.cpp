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

#include <DHTSensor.h>

#define STATE_MASTER    0
#define STATE_START     1
#define STATE_READ      2

DHTSensor::DHTSensor(int _pin, byte _type, byte _count) :
    m_pin(_pin),
    m_state(STATE_MASTER),
    m_type(_type),
    m_count(_count) {
    // nothing left to write here
} // constructor

void DHTSensor::setup() {
    // set up the pins!
    pinMode(m_pin, INPUT);
    digitalWrite(m_pin, HIGH); // pullup
    wait(0);
} // setup

void DHTSensor::run(byte _serv) {
    byte laststate = HIGH;
    byte counter = 0;
    byte j = 0, i;

    switch(m_state) {
        case STATE_MASTER:
            m_data[0] = m_data[1] = m_data[2] = m_data[3] = m_data[4] = 0;
            // pull the pin high and wait 250 milliseconds
            pinMode(m_pin, OUTPUT);
            digitalWrite(m_pin, HIGH);
            m_state = STATE_START;
            wait(250);
            break;
        case STATE_START:
            // now pull it low for ~20 milliseconds
            pinMode(m_pin, OUTPUT);
            digitalWrite(m_pin, LOW);
            m_state = STATE_READ;
            wait(20);
            break;
        case STATE_READ:
            digitalWrite(m_pin, HIGH);
            delayMicroseconds(40);
            pinMode(m_pin, INPUT);

            // read in timings
            for ( i = 0; i < MAXTIMINGS; i++) {
                counter = 0;
                while (digitalRead(m_pin) == laststate) {
                    counter++;
                    delayMicroseconds(1);
                    if (counter == 255) {
                        break;
                    }
                }
                laststate = digitalRead(m_pin);

                if (counter == 255) break;

                // ignore first 3 transitions
                if ((i >= 4) && (i%2 == 0)) {
                    // shove each bit into the storage bytes
                    m_data[j/8] <<= 1;
                    if (counter > m_count)
                      m_data[j/8] |= 1;
                    j++;
                }

            } // for

            // check we read 40 bits and that the checksum matches
            if ((j >= 40) && 
                (m_data[4] == ((m_data[0] + m_data[1] + m_data[2] + m_data[3]) & 0xFF)) ) {
                event(EVENT_DHT_READY);
            } else {
                event(EVENT_DHT_ERROR);
            }
            m_state = STATE_MASTER;
            wait(5000); // wait 5s
            break;
    }
} // run

float DHTSensor::temperature() {
    float f;
    switch (m_type) {
        case DHT11:
              f = m_data[2];
              return f;
        case DHT22:
        case DHT21:
              f = m_data[2] & 0x7F;
              f *= 256;
              f += m_data[3];
              f /= 10;
              if (m_data[2] & 0x80)
                  f *= -1;
              return f;
    }
} // temperature

float DHTSensor::humidity() {
    float f;
    switch (m_type) {
        case DHT11:
            f = m_data[0];
            return f;
        case DHT22:
        case DHT21:
            f = m_data[0];
            f *= 256;
            f += m_data[1];
            f /= 10;
            return f;
    } 
} // humidity
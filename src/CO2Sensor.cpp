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

#include <CO2Sensor.h>

const byte cmd_gaz_concentration [] =    {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };  // 0x86
const byte cmd_calibrate_zero_point [] = {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78 };  // 0x87
const byte cmd_calibrate_span_point [] = {0xff, 0x01, 0x88, 0x07, 0xD0, 0x00, 0x00, 0x00, 0xA0 };  // 0x88

CO2Sensor::CO2Sensor(int _txpin, int _rxpin) :
    m_co2ppm(0),
    m_temperature(0),
    m_reading(false),
    m_hot(false),
    m_serial(_txpin, _rxpin) {
    // nothing left to write here
} // constructor

void CO2Sensor::setup() {
    m_serial.begin(9600);
    wait(30000, SERVICE_WARMUP);   // check every 30s if sensor is warm enough
    wait(10000, SERVICE_READING);  // reading gas concentration every 10s
} // setup

void CO2Sensor::run(byte _serv) {
    byte data[9];
    int i;

    switch (_serv) {    
        case SERVICE_READING:
            if (m_reading) {
                if(m_serial.available()) {
                    while(m_serial.available()) {
                        for(i = 0; i < 9; i++) data[i] = m_serial.read();
                    }
                }
                m_serial.ignore(); // disable RX interrupts
                
                // if frame size and checksum is ok
                if((i == 9) && (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) == data[8]) {
                    m_co2ppm = (int)data[2] * 256 + (int)data[3];
                    m_temperature = (int)data[4] - 40;
                    event(EVENT_CO2_DATA);
                }
                // next reading
                m_reading = false;
                wait(10000, SERVICE_READING);    
            } else {
                // transmit command data
                for(i = 0; i < 9; i++) m_serial.write(cmd_gaz_concentration[i]);
                m_reading = true;
                m_serial.listen(); // enable RX interrupts
                wait(10, SERVICE_READING);        
            }
            break;
        case SERVICE_WARMUP:
            if (m_temperature > 22) {
                m_hot = true;
                event(EVENT_CO2_READY);
            } else {
                event(EVENT_CO2_WARMING);
                wait(30000, SERVICE_WARMUP);
            }
            break;
    }
} // run

bool CO2Sensor::isReady() {
    return m_hot;
} // isReady

int CO2Sensor::concentration() {
    return m_co2ppm;
} // concentration

int CO2Sensor::temperature() {
    return m_temperature;
} // temperature
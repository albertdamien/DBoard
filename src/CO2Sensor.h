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

#ifndef _CO2_SENSOR_H_
#define _CO2_SENSOR_H_

#include <Arduino.h>
#include <Component.h>
#include <NeoSWSerial.h>

// The hardware is a MH-Z16 Intelligent Infrared Gas Module 
// communicating via UART

#define EVENT_CO2_WARMING 0
#define EVENT_CO2_READY   1
#define EVENT_CO2_DATA    2

// Run services
#define SERVICE_WARMUP  0x00 // warm up service
#define SERVICE_READING 0x01 // reading service

class CO2Sensor : public Component {
    public:
        CO2Sensor(int txpin, int rxpin);
        void setup();
        void run(byte);
        bool isReady();
        int temperature();
        int concentration();
        
    protected:
        int         m_co2ppm;
        int         m_temperature;
        int         m_reading;
        bool        m_hot;
        NeoSWSerial m_serial; 
};

#endif // _CO2_SENSOR_H_
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

#ifndef _DHT_SENSOR_H_
#define _DHT_SENSOR_H_

#include <Arduino.h>
#include <Component.h>

// The hardware is a AM2302 (Grove Temperature & Humidity Sensor)
// communicating via Single Wire implementation (see specification)
// MIT license inspired by code written by Adafruit Industries 

// 8 MHz(ish) AVR ---------------------------------------------------------
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
#define COUNT 3
// 16 MHz(ish) AVR --------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
#define COUNT 6
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

// How many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS  85

#define DHT11       11
#define DHT22       22
#define DHT21       21
#define AM2301      21

// Component Events
#define EVENT_DHT_READY  0
#define EVENT_DHT_ERROR  1

class DHTSensor : public Component {
    public:
        DHTSensor(int pin, byte type, byte count = COUNT);
        void  setup();
        void  run(byte);
        float temperature();
        float humidity();
        
    private:
        byte m_data[6];
        byte m_state;        // state of the reading
        byte m_pin;          // pin number
        byte m_type;         // type of sensor
        byte m_count;        // correspond to CPU speed
};

#endif // _DHT_SENSOR_H_
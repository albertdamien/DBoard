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

#ifndef _AIR_QUALITY_SENSOR_H_
#define _AIR_QUALITY_SENSOR_H_

#include <Arduino.h>
#include <Component.h>
#include <Input.h>

// The hardware is a MP-503 Air Quality Gas Sensor 

#define EVENT_AIR_READY             0
#define EVENT_AIR_WARMUP            1
#define EVENT_AIR_ERROR             2
#define EVENT_AIR_POLLUTION_HIGH    3
#define EVENT_AIR_POLLUTION_MEDIUM  4
#define EVENT_AIR_POLLUTION_LOW     5
#define EVENT_AIR_FRESH_AIR         6

class AirQualitySensor : public Input {
    public:
        AirQualitySensor(int pin);
        void run(byte);
        int  value();
        bool isReady();
        
    private:
        int m_value;
        int m_avg_value;
        int m_avg_total;
        int m_avg_index;
        int m_state;
        int m_init_retry;
};

#endif // _AIR_QUALITY_SENSOR_H_
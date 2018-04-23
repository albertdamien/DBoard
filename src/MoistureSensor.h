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

#ifndef _MOISTURE_SENSOR_H_
#define _MOISTURE_SENSOR_H_

#include <Arduino.h>
#include <Input.h>

#define EVENT_MOISTURE_DRY      0
#define EVENT_MOISTURE_HUMID    1
#define EVENT_MOISTURE_WATER    2

class MoistureSensor : public Input {
    public:
        MoistureSensor(int pin);
        void  run(serviceType);     // update value   
        void  setup();              // initialize the detector
        int   value();              // value of brightness
        void  probe(int delay);     // start probing the moisture /!\ beware of sensor lifetime
        void  stop();               // stop probing
        
    private:
        int   m_value;              // previous value
        int   m_delay;              // survey cycle delay
        int   m_threshold;          // threshold between values
};

#endif // _MOISTURE_SENSOR_H_
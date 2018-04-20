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

#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

#include <Arduino.h>
#include <Input.h>

#define EVENT_LUX_DAY     0
#define EVENT_LUX_NIGHT   1
#define EVENT_LUX_CHANGE  2

class LightSensor : public Input {
    public:
        LightSensor(int pin);
        void  run(serviceType);     // update value   
        void  setup();              // initialize the detector
        int   value();              // value of brightness
        
    private:
        bool  m_day;                // is is daylight or dark night ?
        int   m_value;              // stored value
        int   m_thresholdDayLight;  // threshold between daylight and night
        int   m_changeMillis;       // timestamp for detection
};

#endif // _LIGHT_SENSOR_H_
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

#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

#include <Arduino.h>
#include <Input.h>

#define EVENT_TEMP_HOT     0
#define EVENT_TEMP_COLD    1
#define EVENT_TEMP_CHANGE  2

class TemperatureSensor : public Input {
    public:
        TemperatureSensor(int pin);
        void  run(serviceType);                 // update value    
        float value();                          // value of T°
        void  tune(float _hot, float _cold);    // set the hot/cold threshold

    private:
        float m_value;          // T° value
        float m_hotThreshold;   // hot T° threshold
        float m_coldThreshold;  // cold T° threshold
};

#endif // _TEMPERATURE_SENSOR_H_
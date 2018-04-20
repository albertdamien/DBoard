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

#ifndef _PULSE_LED_H_
#define _PULSE_LED_H_

#include <Arduino.h>
#include <Led.h>

class PulseLed : public Led {
    public:
        PulseLed(int pin);
        void run(serviceType);
        void pulse(float);
        
    private:
        float m_angle;       // angle value (Radian)
        float m_frequency;   // pulsation frequency (Hertz)    
};

#endif // _PULSE_LED_H_
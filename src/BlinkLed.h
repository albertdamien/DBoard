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

#ifndef _BLINK_LED_H_
#define _BLINK_LED_H_

#include <Arduino.h>
#include <Led.h>

class BlinkLed : public Led {
    public:
        BlinkLed(int pin);
        void run(serviceType);
        void blink(long on, long off);
        
    private:
        unsigned long m_onTime;     // milliseconds of on-time
        unsigned long m_offTime;    // milliseconds of off-time     
};

#endif // _BLINK_LED_H_
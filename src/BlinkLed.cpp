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

#include <BlinkLed.h>

BlinkLed::BlinkLed(int _pin) : 
    Led(_pin) {
    // nothing more
} // constructor

void BlinkLed::run(serviceType _serv) {

/**

 Blinking model
  __        
 |  |      
_|  |______
  1    2        

  1 : onTime
  2 : offTime


**/

    if(m_ledState == HIGH)
    {
        setOff();
        wait(m_offTime);
    }
    else if (m_ledState == LOW)
    {
        setOn();
        wait(m_onTime);
    }
} // run

void BlinkLed::blink(long _on, long _off) {
    m_onTime = _on;
    m_offTime = _off;
    leave();
    wait(0);
} // blink
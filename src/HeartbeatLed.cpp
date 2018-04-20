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

#include <HeartbeatLed.h>

HeartbeatLed::HeartbeatLed(int _pin, long _beat, long _dias, long _syst) :
	Led(_pin),
    m_beatTime(_beat),
    m_diasTime(_dias),
    m_systTime(_syst),
    m_state(0) {
    // nothing more
} // constructor

void HeartbeatLed::run(serviceType _serv) {

/**

 Heartbeat model
  __        __
 |  |      |  |
_|  |______|  |___________
  1    2           3

  1 : beatTime
  2 : diasTime
  3 : systTime

**/

    switch(m_state) {
        case(0):
            setOn();
            wait(m_beatTime);
            break;
        case(1):
            setOff();
            wait(m_diasTime);
            break;
        case(2):
            setOn();
            wait(m_beatTime);
            break;
        case(3):
            setOff();
            wait(m_systTime);
    }
    m_state = (m_state + 1) % 4;
} // run

void HeartbeatLed::beat() {
    leave();
    wait(0);
} // beat
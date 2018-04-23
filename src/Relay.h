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

#ifndef _RELAY_H_
#define _RELAY_H_

#include <Arduino.h>
#include <Component.h>

class Relay : public Component {
    public:
        Relay(int pin);
        ~Relay();
        void setup();
        void run(byte);
        void open();
        void close();
        
    protected:
        int m_relayPin;   // the number of the Relay pin
};

#endif // _RELAY_H_
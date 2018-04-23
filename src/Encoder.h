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

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <Arduino.h>
#include <Component.h>

#define EVENT_ENC_CW        0
#define EVENT_ENC_CCW       1

class Encoder : public Component {
	public:
	    Encoder(int pinA, int pinB);
	    void setup();
	    void run(serviceType);
      
  	private:
        byte m_value;   // the previous value
        byte m_state;   // state of the state machine
	    int  m_pinA;    // input pin A
        int  m_pinB;    // input pin B
};

#endif // _ENCODER_H_
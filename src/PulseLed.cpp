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

#include <PulseLed.h>

PulseLed::PulseLed(int _pin) : 
    Led(_pin),
    m_angle(0) {
    // nothing more
} // constructor

void PulseLed::run(serviceType _serv) {

    const float TWOPI = 6.28318530;
    const int SAMPLES = 20;

    setDim((int)(sin(m_angle) * 127.5 + 127.5));
    m_angle += TWOPI / SAMPLES;
    if (m_angle > TWOPI) m_angle = 0;
    wait(1000/(m_frequency * SAMPLES));
} // run

void PulseLed::pulse(float _freq) {
    m_frequency = _freq;
    leave();
    wait(0);
} // pulse
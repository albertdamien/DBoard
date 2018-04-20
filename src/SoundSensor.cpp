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

#include <SoundSensor.h>

SoundSensor::SoundSensor(int _pin) : 
    Input(_pin, false),
    m_loudThreshold(1024),
    m_quietThreshold(0),
    m_value(0) {
    // nothing more
} // constructor

void SoundSensor::run(serviceType _serv) {
    m_value = read();                  
    //Serial.println(m_value);
    wait(100);
} // run

float SoundSensor::value() {
    return m_value;
} // value

void SoundSensor::onLoud(int _threshold, onSoundHandler _handler) {
    if (_handler) {
        m_loudThreshold = _threshold;
        m_onLoudHandler = _handler;
    }
} // onLoud

void SoundSensor::onQuiet(int _threshold, onSoundHandler _handler) {
    if (_handler) {
        m_quietThreshold = _threshold;
        m_onQuietHandler = _handler;
    }
} // onQuiet
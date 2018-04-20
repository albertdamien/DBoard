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

#ifndef _SOUND_SENSOR_H_
#define _SOUND_SENSOR_H_

#include <Arduino.h>
#include <Input.h>

typedef void (*onSoundHandler)();

class SoundSensor : public Input {
    public:
        SoundSensor(int pin);
        void    run(serviceType);               // update value    
        float   value();                        // value of T°
        void    onLoud(int, onSoundHandler);    // add an handler for Hot T°  
        void    onQuiet(int, onSoundHandler);   // add an handler for cold T°    
        
    private:
        int m_value;                            // T° value
        int m_loudThreshold;                    // hot T° threshold
        int m_quietThreshold;                   // cold T° threshold
        onSoundHandler m_onLoudHandler;         // hot T° handler
        onSoundHandler m_onQuietHandler;        // cold T° handler
};

#endif // _SOUND_SENSOR_H_
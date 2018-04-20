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

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>

// Sensors Custom Character definitions

const byte tempDown[8] = 
    { 0x0E, 0x0E, 0x0E, 0x0E, 0x1F, 0x1F, 0x1F, 0x0E };

const byte tempUp[8] = 
    { 0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x0E };

const byte lux[8] = 
    { 0x00, 0x11, 0x0A, 0x15, 0x0A, 0x11, 0x00, 0x00 };

const byte co2_1[8] = 
    { 0x03, 0x04, 0x04, 0x04, 0x03, 0x00, 0x00, 0x00 };

const byte co2_2[8] = 
    { 0x08, 0x14, 0x14, 0x14, 0x0B, 0x01, 0x02, 0x03 };

const byte drop[8] = 
    { 0x04, 0x04, 0x0A, 0x0A, 0x11, 0x1F, 0x1F, 0x0E };

const byte air_0[8] = 
    { 0x00, 0x04, 0x0A, 0x02, 0x04, 0x00, 0x04, 0x00 };

const byte air_1[8] = 
    { 0x00, 0x0A, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00 };

const byte air_2[8] = 
    { 0x00, 0x00, 0x0A, 0x00, 0x00, 0x1F, 0x00, 0x00 };

const byte air_3[8] = 
    { 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0E, 0x11, 0x00 };

const byte air_4[8] = 
    { 0x00, 0x00, 0x0E, 0x15, 0x1B, 0x0E, 0x0E, 0x00 };

#endif // _SENSOR_H_
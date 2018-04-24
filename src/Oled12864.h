/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  SSD130x OLED Driver Library from Visweswara R
  Copyright (c) 2011 seeed technology inc.

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

#ifndef _OLED12864_H_
#define _OLED12864_H_

#include <Arduino.h>
#include <Component.h>

#define EVENT_READY 0

// SSD130x Instruction set addresses
#define OLED12864_HORIZONTAL_MODE   0x00
#define OLED12864_PAGE_MODE         0x02

#define OLED12864_ADDRESS           0x3C
#define OLED12864_COMMAND_MODE      0x80
#define OLED12864_DATA_MODE         0x40
#define OLED12864_ADDRESSING_MODE   0x20
#define OLED12864_DISPLAY_OFF       0xAE
#define OLED12864_DISPLAY_ON        0xAF
#define OLED12864_NORMAL_DISPLAY    0xA6
#define OLED12864_INVERSE_DISPLAY   0xA7
#define OLED12864_ACTIVATE_SCROLL   0x2F
#define OLED12864_DEACTIVATE_SCROLL 0x2E
#define OLED12864_RIGHT_SCROLL      0x26
#define OLED12864_LEFT_SCROLL       0x27
#define OLED12864_SET_BRIGHTNESS    0x81

#define Scroll_Left                 0x00
#define Scroll_Right                0x01

#define Scroll_2Frames              0x07
#define Scroll_3Frames              0x04
#define Scroll_4Frames              0x05
#define Scroll_5Frames              0x00
#define Scroll_25Frames             0x06
#define Scroll_64Frames             0x01
#define Scroll_128Frames            0x02
#define Scroll_256Frames            0x03

class Oled12864 : public Component {
    public:
        Oled12864();
        void setup();
        void run(byte);
        void setNormalDisplay();
        void setInverseDisplay();
        void setPageMode();
        void setHorizontalMode();
        void setTextXY(byte row, byte column);
        void clearDisplay();
        void setBrightness(byte Brightness);
        void putChar(byte c);
        void putString(const byte string[]);
        byte putNumber(long number);
        byte putFloat(float number, byte decimal);
        byte putFloat(float number);
        void drawBitmap(byte *bitmap, int bytes);
        void setScrollProperties(bool direction, byte startPage, byte endPage, byte scrollSpeed);
        void activateScroll();
        void deactivateScroll();
        
    private:
        void sendCommand(byte command);
        void sendData(byte data);
        char m_addressingMode;
        byte m_state;

};

#endif // _OLED12864_H_
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

#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <Component.h>
#include <Wire.h>
#include <inttypes.h>
#include "Print.h"

// Color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04   // pwm2
#define REG_GREEN       0x03   // pwm1
#define REG_BLUE        0x02   // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME  0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class Lcd : public Component, public Print {
    public:
        Lcd();
        void setup();
        void run(serviceType);
        void animateChar(uint8_t location, uint8_t chartabsize, uint8_t chartab[][8], long delay, bool repeat);
        void noAnimateChar(uint8_t location);
        void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
        void clear();
        void home();
        void noDisplay();
        void display();
        void noBlink();
        void blink();
        void noCursor();
        void cursor();
        void scrollDisplayLeft();
        void scrollDisplayRight();
        void leftToRight();
        void rightToLeft();
        void autoscroll();
        void noAutoscroll();
        void createChar(uint8_t, uint8_t[]);
        void setCursor(uint8_t, uint8_t); 
        virtual size_t write(uint8_t);
        void command(uint8_t);
        
        // color control
        void fadeTo(byte delay, unsigned char r, unsigned char g, unsigned char b); // fade current to wanted color
        void setRGB(unsigned char r, unsigned char g, unsigned char b); // set rgb
        void setPWM(unsigned char color, unsigned char pwm);            // set pwm
        void setColor(unsigned char color);
        void setColorAll(){setRGB(0, 0, 0);}
        void setColorWhite(){setRGB(255, 255, 255);}
        using Print::write;
        
    private:
        void send(uint8_t, uint8_t);
        void setReg(unsigned char addr, unsigned char dta);
        uint8_t m_displayfunction;
        uint8_t m_displaycontrol;
        uint8_t m_displaymode;
        uint8_t m_numlines;
        uint8_t m_currline;
        uint8_t m_anim_location;
        uint8_t m_anim_tabsize;
        uint8_t m_anim_tab_index;
        uint8_t (*m_anim_tab)[8];
        long    m_anim_delay;
        bool    m_anim_repeat;
        byte    m_current_red;
        byte    m_current_green;
        byte    m_current_blue;
        byte    m_wanted_red;
        byte    m_wanted_green;
        byte    m_wanted_blue;
        byte    m_fade_delay;
};

#endif // _LCD_H_
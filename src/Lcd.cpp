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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Wire.h>
#include <Lcd.h>

// Device I2C Arress
#define LCD_ADDRESS     (0x7c>>1)
#define RGB_ADDRESS     (0xc4>>1)

// Run services
#define SERVICE_CHAR    0x00    // animate char
#define SERVICE_FADE    0x01    // fading RGB

void i2c_send_byte(unsigned char dta) {
    Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
    Wire.write(dta);                            // sends five bytes
    Wire.endTransmission();                     // stop transmitting
} // i2c_send_byte

void i2c_send_byteS(unsigned char *dta, unsigned char len) {
    Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
    for(int i=0; i<len; i++)
    {
        Wire.write(dta[i]);
    }
    Wire.endTransmission();                     // stop transmitting
} // i2c_send_byteS

Lcd::Lcd() {
  // nothing here
} // Lcd

void Lcd::setup() {
    begin(16, 2);
} // setup

void Lcd::run(serviceType _serv) {
    switch (_serv) {    
        case SERVICE_CHAR:
            createChar(m_anim_location, m_anim_tab[m_anim_tab_index]);
            m_anim_tab_index = ++m_anim_tab_index % m_anim_tabsize;
            if ((m_anim_tab_index > 0) || m_anim_repeat) wait(m_anim_delay, SERVICE_CHAR);
            break;
        case SERVICE_FADE:
            if ((int)(m_wanted_red - m_current_red) > 4)             setPWM(REG_RED, m_current_red + 4);
            else if ((int)(m_wanted_red - m_current_red) > 0)        setPWM(REG_RED, m_current_red + 1);
            else if ((int)(m_current_red - m_wanted_red) > 4)        setPWM(REG_RED, m_current_red - 4);
            else if ((int)(m_current_red - m_wanted_red) > 0)        setPWM(REG_RED, m_current_red - 1);

            if ((int)(m_wanted_green - m_current_green) > 4)         setPWM(REG_GREEN, m_current_green + 4);
            else if ((int)(m_wanted_green - m_current_green) > 0)    setPWM(REG_GREEN, m_current_green + 1);
            else if ((int)(m_current_green - m_wanted_green) > 4)    setPWM(REG_GREEN, m_current_green - 4);
            else if ((int)(m_current_green - m_wanted_green) > 0)    setPWM(REG_GREEN, m_current_green - 1);

            if ((int)(m_wanted_blue - m_current_blue) > 4)           setPWM(REG_BLUE, m_current_blue + 4);
            else if ((int)(m_wanted_blue - m_current_blue) > 0)      setPWM(REG_BLUE, m_current_blue + 1);
            else if ((int)(m_current_blue - m_wanted_blue) > 4)      setPWM(REG_BLUE, m_current_blue - 4);
            else if ((int)(m_current_blue - m_wanted_blue) > 0)      setPWM(REG_BLUE, m_current_blue - 1);

            if ((m_wanted_red != m_current_red) || \
                (m_wanted_green != m_current_green) || \
                (m_wanted_blue != m_current_blue)) wait(m_fade_delay, SERVICE_FADE);
            break;
    }
} // run

void Lcd::fadeTo(byte _delay, unsigned char r, unsigned char g, unsigned char b) {
    leave(SERVICE_FADE);
    m_wanted_red    = r;
    m_wanted_green  = g;
    m_wanted_blue   = b;
    m_fade_delay = _delay;
    wait(0, SERVICE_FADE);
} // fadeTo

void Lcd::animateChar(uint8_t _location, uint8_t _chartabsize, uint8_t _chartab[][8], long _delay, bool _repeat) {
    leave(SERVICE_CHAR);
    m_anim_location = _location;
    m_anim_tabsize = _chartabsize;
    m_anim_tab = _chartab;
    m_anim_tab_index = 0;
    m_anim_delay = _delay;
    m_anim_repeat = _repeat;
    wait(0, SERVICE_CHAR);
} // animateChar

void Lcd::noAnimateChar(uint8_t _location) {
    leave(SERVICE_CHAR);
} // noAnimateChar

void Lcd::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    Wire.begin();
    
    if (lines > 1) {
        m_displayfunction |= LCD_2LINE;
    }
    m_numlines = lines;
    m_currline = 0;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        m_displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    delayMicroseconds(50000);

    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | m_displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | m_displayfunction);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | m_displayfunction);

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | m_displayfunction);

    // turn the display on with no cursor or blinking default
    m_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    m_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | m_displaymode);
    
    // backlight init
    setReg(0, 0);
    setReg(1, 0);
    setReg(0x08, 0xAA);     // all led control by pwm
    
    setColorWhite();
} // begin

/********** high level commands, for the user! */
void Lcd::clear() {
    command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
    delayMicroseconds(2000);          // this command takes a long time!
} // clear

void Lcd::home() {
    command(LCD_RETURNHOME);        // set cursor position to zero
    delayMicroseconds(2000);        // this command takes a long time!
} // home

void Lcd::setCursor(uint8_t col, uint8_t row) {
    col = (row == 0 ? col|0x80 : col|0xc0);
    unsigned char dta[2] = {0x80, col};
    i2c_send_byteS(dta, 2);
} // setCursor

// Turn the display on/off (quickly)
void Lcd::noDisplay() {
    m_displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // noDisplay

void Lcd::display() {
    m_displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // display

// Turns the underline cursor on/off
void Lcd::noCursor() {
    m_displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // noCursor

void Lcd::cursor() {
    m_displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // cursor

// Turn on and off the blinking cursor
void Lcd::noBlink() {
    m_displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // noBlink

void Lcd::blink() {
    m_displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | m_displaycontrol);
} // blink

// These commands scroll the display without changing the RAM
void Lcd::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
} // scrollDisplayLeft

void Lcd::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
} // scrollDisplayRight

// This is for text that flows Left to Right
void Lcd::leftToRight(void) {
    m_displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | m_displaymode);
} // leftToRight

// This is for text that flows Right to Left
void Lcd::rightToLeft(void) {
    m_displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | m_displaymode);
} // rightToLeft

// This will 'right justify' text from the cursor
void Lcd::autoscroll(void) {
    m_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | m_displaymode);
} // autoscroll

// This will 'left justify' text from the cursor
void Lcd::noAutoscroll(void) {
    m_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | m_displaymode);
} // noAutoscroll

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Lcd::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    
    unsigned char dta[9];
    dta[0] = 0x40;
    for(int i=0; i<8; i++)
    {
        dta[i+1] = charmap[i];
    }
    i2c_send_byteS(dta, 9);
} // createChar

/*********** mid level commands, for sending data/cmds */

// send command
inline void Lcd::command(uint8_t value) {
    unsigned char dta[2] = {0x80, value};
    i2c_send_byteS(dta, 2);
} // command

// send data
inline size_t Lcd::write(uint8_t value) {

    unsigned char dta[2] = {0x40, value};
    i2c_send_byteS(dta, 2);
    return 1; // assume sucess
} // write

void Lcd::setReg(unsigned char addr, unsigned char dta) {
    
    Wire.beginTransmission(RGB_ADDRESS); // transmit to device #4
    Wire.write(addr);
    Wire.write(dta);
    Wire.endTransmission();    // stop transmitting
} // setReg

void Lcd::setRGB(unsigned char r, unsigned char g, unsigned char b) {
    // Store the value of RGB backlight
    m_current_red = r;
    m_current_green = g;
    m_current_blue = b;
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
} // setRGB

void Lcd::setPWM(unsigned char color, unsigned char pwm) {
    // Store the value of RGB backlight
    if (color == REG_RED) m_current_red = pwm;
    if (color == REG_GREEN) m_current_green = pwm;
    if (color == REG_BLUE) m_current_blue = pwm;
    setReg(color, pwm);
} // setPWM

const unsigned char color_define[4][3] = {
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

void Lcd::setColor(unsigned char color) {
    if(color > 3)return ;
    setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
} // setColor
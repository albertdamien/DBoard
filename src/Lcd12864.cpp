/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Adapted from Lcd12864.h - Lcd12864 library Version 1.22
  Copyright (C), 2016, Sparking Work Space. All right reserved.

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

#include <Lcd12864.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// Device I2C Address
#define I2C_LCD_ADDRESS 0x51

// Run services
#define SERVICE_CHAR    0x00    // animate char
#define SERVICE_FADE    0x01    // fading RGB

/*const Font_Info_t fontInfo[I2C_LCD_NUM_OF_FONT] = 
{
    Font_6x8, 6, 8,
    Font_6x12, 6, 12,
    Font_8x16_1, 8, 16,
    Font_8x16_2, 8, 16,
    Font_10x20, 10, 20,
    Font_12x24, 12, 24,
    Font_16x32, 16, 32,
};*/

/********************* The start of Lcd12864 basic driver APIs *******************/

Lcd12864::Lcd12864() {
    // nothing here
} // constructor

void Lcd12864::setup() {
    I2C_BUS_Init();
} // setup

void Lcd12864::run(serviceType _serv) {
    ;
} // run

/**************************************************************
      Read one byte from device register.
***************************************************************/
uint8_t Lcd12864::readByteFromReg(enum LCD_RegAddress regAddr) {
    int8_t waitTime;
    I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS); 
    I2C_BUS_WriteByte(regAddr);       
    I2C_BUS_EndTransmission();   

    I2C_BUS_RequestFrom((int16_t)I2C_LCD_ADDRESS,1);
    for(waitTime=10; !I2C_BUS_Available()&&waitTime; waitTime--)
        __asm__("nop\n\t");
    if(I2C_BUS_Available()) {
        I2C_BUS_EndTransmission();
        return I2C_BUS_ReadByte();
    }
    else
        return 0;
} // readByteFromReg

/**************************************************************
      Write one byte to device register.
***************************************************************/
void Lcd12864::writeByteToReg(enum LCD_RegAddress regAddr, uint8_t buf) {
    I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS); 
    I2C_BUS_WriteByte(regAddr);      
    I2C_BUS_WriteByte(buf);
    I2C_BUS_EndTransmission();  
} // writeByteToReg

/**************************************************************
      Read multiple bytes from device register.
***************************************************************/
void Lcd12864::readSeriesFromReg(enum LCD_RegAddress regAddr, uint8_t *buf, int8_t length) {
    int8_t i, waitTime;
    I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS); 
    I2C_BUS_WriteByte(regAddr);      
    I2C_BUS_EndTransmission();   
    if(length>32)
        length=32;
    I2C_BUS_RequestFrom((int16_t)I2C_LCD_ADDRESS,length);
    for(waitTime=10; !I2C_BUS_Available()&&waitTime; waitTime--)
        __asm__("nop\n\t");
    for(i=0; i<length; i++) {
        buf[i] = I2C_BUS_ReadByte();
        //Serial.print(buf[i], HEX);
        //Serial.print(" ");
        for(waitTime=10; !I2C_BUS_Available()&&waitTime; waitTime--)
            __asm__("nop\n\t");
    }
    I2C_BUS_EndTransmission();
} // readSeriesFromReg

/**************************************************************
      Write multiple bytes to device register.
***************************************************************/
void Lcd12864::writeSeriesToReg(enum LCD_RegAddress regAddr, const uint8_t *buf, uint8_t length) {
    uint8_t i;
    I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS);
    I2C_BUS_WriteByte(regAddr);       
    for(i=0; i<length; i++) I2C_BUS_WriteByte(buf[i]);
    I2C_BUS_EndTransmission();    // stop transmitting
} // writeSeriesToReg

/********************* The end of Lcd12864 basic driver APIs *******************/

#ifdef  SUPPORT_FULL_API_LIB
#if  SUPPORT_FULL_API_LIB == TRUE

void Lcd12864::charGotoXY(uint8_t x, uint8_t y) {
    uint8_t buf[2];
    buf[0]=x;
    buf[1]=y;
    writeSeriesToReg(CharXPosRegAddr, buf, 2);
} // charGotoXY

void Lcd12864::readRAMGotoXY(uint8_t x, uint8_t y) {
    uint8_t buf[2];
    buf[0]=x;
    buf[1]=y;
    writeSeriesToReg(ReadRAM_XPosRegAddr, buf, 2);
} // readRAMGotoXY

void Lcd12864::writeRAMGotoXY(uint8_t x, uint8_t y) {
    uint8_t buf[2];
    buf[0]=x;
    buf[1]=y;
    writeSeriesToReg(WriteRAM_XPosRegAddr, buf, 2);
} // writeRAMGotoXY

/*void Lcd12864::sendBitmapData(const uint8_t *buf, uint8_t length)
{
    uint8_t i;
    I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS); // transmit device adress
    I2C_BUS_WriteByte(DisRAMAddr);        //  transmit register adress to the device
    for(i=0; i<length; i++)
    {
        I2C_BUS_WriteByte(pgm_read_byte_near(buf++)); 
    }
    I2C_BUS_EndTransmission();    // stop transmitting
}*/

void Lcd12864::sendBitmapData(const uint8_t *buf, uint8_t length) {
    uint8_t i;
    uint16_t circleTimes, circleCounter, transBytesNum;
    
    circleTimes = length/I2C_LCD_TRANS_ONCE_BYTE_MAX + 1;
    
    for(circleCounter = 0; circleCounter < circleTimes; circleCounter ++) {
        if(circleCounter+1 >= circleTimes)
          transBytesNum = length%I2C_LCD_TRANS_ONCE_BYTE_MAX;
        else
          transBytesNum = I2C_LCD_TRANS_ONCE_BYTE_MAX;
        
        I2C_BUS_BeginTransmission(I2C_LCD_ADDRESS); // transmit device adress
        I2C_BUS_WriteByte(DisRAMAddr);        //  transmit register adress to the device
        for(i = 0; i < transBytesNum; i ++) I2C_BUS_WriteByte(pgm_read_byte_near(buf++)); 
        I2C_BUS_EndTransmission();    // stop transmitting
    }
} // sendBitmapData

void Lcd12864::fontModeConf(enum LCD_FontSort font, enum LCD_FontMode mode, enum LCD_CharMode cMode) {
    writeByteToReg(FontModeRegAddr,cMode|mode|font);
} // fontModeConf

void Lcd12864::dispCharAt(char buf, uint8_t x, uint8_t y) {
    charGotoXY(x,y);
    writeByteToReg(DisRAMAddr, buf);
} // dispCharAt

void Lcd12864::dispStringAt(char *buf, uint8_t x, uint8_t y) {
    charGotoXY(x, y);
    for(; *buf; buf++) writeByteToReg(DisRAMAddr, *buf);
} // dispStringAt

const uint8_t fontYsizeTab[I2C_LCD_NUM_OF_FONT] = {8, 12, 16, 16, 20, 24, 32};

inline size_t Lcd12864::write(uint8_t value) {
  uint8_t Y_Present, Y_New, Fontsize_Y, fontIndex;
  switch(value) {
    case 0x0d: 
        break;
    case 0x0a: 
        Y_Present = readByteFromReg(CharYPosRegAddr);
        fontIndex = readByteFromReg(FontModeRegAddr)&0x0f;
        
        if(Y_Present + 2*fontYsizeTab[fontIndex] <= I2C_LCD_Y_SIZE_MAX) {
            Y_New = fontYsizeTab[fontIndex] + Y_Present; 
            charGotoXY(0, Y_New); 
        }
        else
            charGotoXY(0, 0); 
        break;
    case 0x09:
        writeSeriesToReg(DisRAMAddr, (uint8_t *)"  ", 2);
        break;
    default: 
        writeByteToReg(DisRAMAddr, value);
  }  
  return 1; // assume sucess
} // write

void Lcd12864::cursorConf(enum LCD_SwitchState swi, uint8_t freq) {
    writeByteToReg(CursorConfigRegAddr, (char)(swi<<7)|freq);
} // cursorConf

void Lcd12864::cursorGotoXY(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    uint8_t buf[4];
    buf[0]=x;
    buf[1]=y;
    buf[2]=width;
    buf[3]=height;
    writeSeriesToReg(CursorXPosRegAddr, buf, 4);
} // cursorGotoXY

#ifdef  SUPPORT_2D_GRAPHIC_LIB
#if  SUPPORT_2D_GRAPHIC_LIB == TRUE

void Lcd12864::drawDotAt(uint8_t x, uint8_t y, enum LCD_ColorSort color) {
    uint8_t buf[2];
    if(x<128 && y<64) {
        buf[0]=x;
        buf[1]=(uint8_t)(color<<7)|y;
        writeSeriesToReg(DrawDotXPosRegAddr, buf, 2);
    }
} // drawDotAt

void Lcd12864::drawHLineAt(uint8_t startX, uint8_t endX, uint8_t y, enum LCD_ColorSort color) {
    drawLineAt(startX, endX, y, y, color);
} // drawHLineAt

void Lcd12864::drawVLineAt(uint8_t startY, uint8_t endY, uint8_t x, enum LCD_ColorSort color) {
    drawLineAt(x, x, startY, endY, color);
} // drawVLineAt

void Lcd12864::drawLineAt(uint8_t startX, uint8_t endX, uint8_t startY, uint8_t endY, enum LCD_ColorSort color) {
    uint8_t buf[4];
    if(endY<64) {
        buf[0]=startX;
        buf[1]=endX;
        buf[2]=startY;
        buf[3]=(uint8_t)(color<<7)|endY;
        writeSeriesToReg(DrawLineStartXRegAddr, buf, 4);
    }
} // drawLineAt

void Lcd12864::drawRectangleAt(uint8_t x, uint8_t y, uint8_t width, uint8_t height, enum LCD_DrawMode mode) {
    uint8_t buf[5];
    buf[0]=x;
    buf[1]=y;
    buf[2]=width;
    buf[3]=height;
    buf[4]=mode;
    writeSeriesToReg(DrawRectangleXPosRegAddr, buf, 5);
} // drawRectangleAt

void Lcd12864::drawCircleAt(int8_t x, int8_t y, uint8_t r, enum LCD_DrawMode mode) {
    uint8_t buf[4];
    if(x<128 && y<64 && r<64) {
        buf[0]=x;
        buf[1]=y;
        buf[2]=r;
        buf[3]=mode;
        writeSeriesToReg(DrawCircleXPosRegAddr, buf, 4);
    }
} // drawCircleAt

void Lcd12864::drawScreenAreaAt(GUI_Bitmap_t *bitmap, uint8_t x, uint8_t y) {
    uint8_t regBuf[4];
    int16_t byteMax;
    int8_t i,counter;
    const uint8_t *buf = bitmap->pData;
    if(y<64 && x<128) {
        regBuf[0] = x;
        regBuf[1] = y;
        regBuf[2] = bitmap->XSize;
        regBuf[3] = bitmap->YSize;
        writeSeriesToReg(DrawBitmapXPosRegAddr, regBuf, 4);
        byteMax = regBuf[3]*bitmap->BytesPerLine;
        counter = byteMax/31;
        if(counter)
            for(i=0; i<counter; i++,buf+=31)
                sendBitmapData(buf, 31);
        counter = byteMax%31;
        if(counter)
            sendBitmapData(buf, counter);
    }
} // drawScreenAreaAt

void Lcd12864::drawFullScreen(const uint8_t *buf) {
    uint8_t i;
    writeRAMGotoXY(0,0);
    for(i=0; i<1024; i++)
        writeByteToReg(DisRAMAddr,buf[i]);
} // drawFullScreen

#endif
#endif

uint8_t Lcd12864::readByteDispRAM(uint8_t x, uint8_t y) {
    readRAMGotoXY(x,y);
    return readByteFromReg(DisRAMAddr);
} // readByteDispRAM

void Lcd12864::readSeriesDispRAM(uint8_t *buf, int8_t length, uint8_t x, uint8_t y) {
    readRAMGotoXY(x,y);
    readSeriesFromReg(DisRAMAddr, buf, length);
} // readSeriesDispRAM

void Lcd12864::writeByteDispRAM(uint8_t buf, uint8_t x, uint8_t y) {
    writeRAMGotoXY(x,y);
    writeByteToReg(DisRAMAddr,buf);
} // writeByteDispRAM

void Lcd12864::writeSeriesDispRAM(uint8_t *buf, int8_t length, uint8_t x, uint8_t y) {
    writeRAMGotoXY(x,y);
    writeSeriesToReg(DisRAMAddr, buf, length);
} // writeSeriesDispRAM

void Lcd12864::displayConf(enum LCD_DisplayMode mode) {
    writeByteToReg(DisplayConfigRegAddr, mode);
} // displayConf

void Lcd12864::workingModeConf(enum LCD_SwitchState logoSwi, enum LCD_SwitchState backLightSwi, enum LCD_WorkingMode mode) {
    writeByteToReg(WorkingModeRegAddr, 0x50|(uint8_t)(logoSwi<<3)|(uint8_t)(backLightSwi<<2)|mode);
} // workingModeConf

void Lcd12864::backlightConf(enum LCD_SettingMode mode, uint8_t buf) {
    if(buf>0x7f)
        buf = 0x7f;
    writeByteToReg(BackLightConfigRegAddr, mode|buf);
} // backlightConf

void Lcd12864::contrastConf(enum LCD_SettingMode mode, uint8_t buf) {
    if(buf>0x3f)
        buf = 0x3f;
    writeByteToReg(ContrastConfigRegAddr, mode|buf);
} // contrastConf

void Lcd12864::deviceAddrEdit(uint8_t newAddr) {
    uint8_t buf[2];
    buf[0]=0x80;
    buf[1]=newAddr;
    writeSeriesToReg(DeviceAddressRegAddr, buf, 2);
} // deviceAddrEdit

void Lcd12864::cleanAll(enum LCD_ColorSort color) {
    uint8_t buf;
    buf = readByteFromReg(DisplayConfigRegAddr);
    if(color == WHITE)
        writeByteToReg(DisplayConfigRegAddr, (buf|0x40)&0xdf);
    else
        writeByteToReg(DisplayConfigRegAddr, buf|0x60);
} // cleanAll

#endif
#endif
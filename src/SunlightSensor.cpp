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

#include <SunlightSensor.h>
#include "Wire.h"

#define STATE_SEARCH    0
#define STATE_FOUND     1
#define STATE_RESET     2
#define STATE_INIT      3
#define STATE_READY     4

SunlightSensor::SunlightSensor() : 
    m_state(STATE_SEARCH) {
    // nothing else to write here
} // constructor

void SunlightSensor::setup() {
    Wire.begin(); // init I2C
    wait(10);
} // setup

void SunlightSensor::run(serviceType _serv) {
    switch(m_state) {
        case STATE_SEARCH:
            if(readByte(SI114X_PART_ID) == 0x45) {
                m_state = STATE_FOUND;
                wait(10);
            } else {
                event(EVENT_SUNLIGHT_NOT_READY);
                wait(1000);
            }
            break;
        case STATE_FOUND:
            writeByte(SI114X_MEAS_RATE0, 0);
            writeByte(SI114X_MEAS_RATE1, 0);
            writeByte(SI114X_IRQ_ENABLE, 0);
            writeByte(SI114X_IRQ_MODE1, 0);
            writeByte(SI114X_IRQ_MODE2, 0);
            writeByte(SI114X_INT_CFG, 0);
            writeByte(SI114X_IRQ_STATUS, 0xFF);
            writeByte(SI114X_COMMAND, SI114X_RESET);
            m_state = STATE_RESET;
            wait(10);
            break;
        case STATE_RESET:
            writeByte(SI114X_HW_KEY, 0x17);
            m_state = STATE_INIT;
            wait(10);
            break;
        case STATE_INIT:
            // enable UV reading  
            writeByte(SI114X_UCOEFF0, 0x29);
            writeByte(SI114X_UCOEFF1, 0x89);
            writeByte(SI114X_UCOEFF2, 0x02);
            writeByte(SI114X_UCOEFF3, 0x00);
            writeParamData(SI114X_CHLIST, SI114X_CHLIST_ENUV | SI114X_CHLIST_ENALSIR | SI114X_CHLIST_ENALSVIS | SI114X_CHLIST_ENPS1);
            // set LED1 current(22.4mA)(It is a normal value for many LED)
            writeParamData(SI114X_PS1_ADCMUX, SI114X_ADCMUX_LARGE_IR);
            writeByte(SI114X_PS_LED21, SI114X_LED_CURRENT_22MA);
            writeParamData(SI114X_PSLED12_SELECT, SI114X_PSLED12_SELECT_PS1_LED1);
            // ps ADC settings
            writeParamData(SI114X_PS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
            writeParamData(SI114X_PS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
            writeParamData(SI114X_PS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE | SI114X_ADC_MISC_ADC_RAWADC); 
            // vis ADC settings
            writeParamData(SI114X_ALS_VIS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
            writeParamData(SI114X_ALS_VIS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
            writeParamData(SI114X_ALS_VIS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
            // IR ADC setting
            writeParamData(SI114X_ALS_IR_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
            writeParamData(SI114X_ALS_IR_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
            writeParamData(SI114X_ALS_IR_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
            // interrupt enable
            writeByte(SI114X_INT_CFG, SI114X_INT_CFG_INTOE);  
            writeByte(SI114X_IRQ_ENABLE, SI114X_IRQEN_ALS);  
            // auto run
            writeByte(SI114X_MEAS_RATE0, 0xFF);
            writeByte(SI114X_COMMAND, SI114X_PSALS_AUTO);
            m_state = STATE_READY;
            event(EVENT_SUNLIGHT_READY);
            wait(10);
        case STATE_READY:
            break;
    }
} // run

// read Visible Value
unsigned int SunlightSensor::readVisible() {
    return readUInt(SI114X_ALS_VIS_DATA0);    
} // readVisible

// read IR Value
unsigned int SunlightSensor::readIR() {
    return readUInt(SI114X_ALS_IR_DATA0);     
} // readIR

// read Proximity Value
unsigned int SunlightSensor::readProximity(byte _psn) {
    return readUInt(_psn);     
} // readProximity
 
// read UV Value
float SunlightSensor::readUV() {
    return (float)(readUInt(SI114X_AUX_DATA0_UVINDEX0)) / 100;     
} // readUV

// read param data
byte SunlightSensor::readParamData(byte _reg) {
    writeByte(SI114X_COMMAND, _reg | SI114X_QUERY);
    return readByte(SI114X_RD);
} // readParamData

// write param data
byte SunlightSensor::writeParamData(byte _reg, byte _value) {
// write Value into PARAMWR reg first
   writeByte(SI114X_WR, _value);
   writeByte(SI114X_COMMAND, _reg | SI114X_SET);
   // Si114X writes value out to PARAM_RD, read and confirm its right
   return readByte(SI114X_RD);
} // writeParamData

// write one byte into Si114x's reg
void SunlightSensor::writeByte(byte _reg, byte _value) {
    Wire.beginTransmission(SI114X_ADDR); 
    Wire.write(_reg); 
    Wire.write(_value); 
    Wire.endTransmission(); 
} // writeByte

// read one byte data from Si114x
byte SunlightSensor::readByte(byte _reg) {
    Wire.beginTransmission(SI114X_ADDR);
    Wire.write(_reg);
    Wire.endTransmission();
    Wire.requestFrom(SI114X_ADDR, 1);  
    return Wire.read();
} // readByte

// read half word (2 bytes) data from Si114x
unsigned int SunlightSensor::readUInt(byte _reg) {
    unsigned int value;
    Wire.beginTransmission(SI114X_ADDR);
    Wire.write(_reg); 
    Wire.endTransmission(); 
    Wire.requestFrom(SI114X_ADDR, 2);
    value = Wire.read();
    value |= (unsigned int)Wire.read() << 8; 
    return value;
} // readUInt
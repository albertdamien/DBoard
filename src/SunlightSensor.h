/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Adapted from Fuhua.Chen (Copyright (c) 2015 seeed technology inc.)
  
  Si114x sensor from SiLabs

  The MIT License (MIT)
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#ifndef _SUNLIGHT_SENSOR_H_
#define _SUNLIGHT_SENSOR_H_

#include <Arduino.h>
#include <Input.h>

#define EVENT_SUNLIGHT_NOT_READY    0
#define EVENT_SUNLIGHT_READY        1

// Registers, Parameters and Commands
//

//
// Commands
//
#define SI114X_QUERY                        0x80
#define SI114X_SET                          0xA0
#define SI114X_NOP                          0x00
#define SI114X_RESET                        0x01
#define SI114X_BUSADDR                      0x02
#define SI114X_PS_FORCE                     0x05
#define SI114X_GET_CAL                      0x12
#define SI114X_ALS_FORCE                    0x06
#define SI114X_PSALS_FORCE                  0x07
#define SI114X_PS_PAUSE                     0x09
#define SI114X_ALS_PAUSE                    0x0A
#define SI114X_PSALS_PAUSE                  0x0B
#define SI114X_PS_AUTO                      0x0D
#define SI114X_ALS_AUTO                     0x0E
#define SI114X_PSALS_AUTO                   0x0F
//
// I2C Registers
//
#define SI114X_PART_ID                      0x00
#define SI114X_REV_ID                       0x01
#define SI114X_SEQ_ID                       0x02
#define SI114X_INT_CFG                      0x03
#define SI114X_IRQ_ENABLE                   0x04
#define SI114X_IRQ_MODE1                    0x05
#define SI114X_IRQ_MODE2                    0x06
#define SI114X_HW_KEY                       0x07
#define SI114X_MEAS_RATE0                   0x08
#define SI114X_MEAS_RATE1                   0x09
#define SI114X_PS_RATE                      0x0A
#define SI114X_PS_LED21                     0x0F
#define SI114X_PS_LED3                      0x10
#define SI114X_UCOEFF0                      0x13
#define SI114X_UCOEFF1                      0x14
#define SI114X_UCOEFF2                      0x15
#define SI114X_UCOEFF3                      0x16
#define SI114X_WR                           0x17
#define SI114X_COMMAND                      0x18
#define SI114X_RESPONSE                     0x20
#define SI114X_IRQ_STATUS                   0x21
#define SI114X_ALS_VIS_DATA0                0x22
#define SI114X_ALS_VIS_DATA1                0x23
#define SI114X_ALS_IR_DATA0                 0x24
#define SI114X_ALS_IR_DATA1                 0x25
#define SI114X_PS1_DATA0                    0x26
#define SI114X_PS1_DATA1                    0x27
#define SI114X_PS2_DATA0                    0x28
#define SI114X_PS2_DATA1                    0x29
#define SI114X_PS3_DATA0                    0x2A
#define SI114X_PS3_DATA1                    0x2B
#define SI114X_AUX_DATA0_UVINDEX0           0x2C
#define SI114X_AUX_DATA1_UVINDEX1           0x2D
#define SI114X_RD                           0x2E
#define SI114X_CHIP_STAT                    0x30
//
// Parameters
//
#define SI114X_CHLIST                       0x01
#define SI114X_CHLIST_ENUV                  0x80
#define SI114X_CHLIST_ENAUX                 0x40
#define SI114X_CHLIST_ENALSIR               0x20
#define SI114X_CHLIST_ENALSVIS              0x10
#define SI114X_CHLIST_ENPS1                 0x01
#define SI114X_CHLIST_ENPS2                 0x02
#define SI114X_CHLIST_ENPS3                 0x04

#define SI114X_PSLED12_SELECT               0x02
#define SI114X_PSLED3_SELECT                0x03

#define SI114X_PS_ENCODE                    0x05
#define SI114X_ALS_ENCODE                   0x06

#define SI114X_PS1_ADCMUX                   0x07
#define SI114X_PS2_ADCMUX                   0x08
#define SI114X_PS3_ADCMUX                   0x09

#define SI114X_PS_ADC_COUNTER               0x0A
#define SI114X_PS_ADC_GAIN                  0x0B
#define SI114X_PS_ADC_MISC                  0x0C

#define SI114X_ALS_IR_ADC_MUX               0x0E
#define SI114X_AUX_ADC_MUX                  0x0F

#define SI114X_ALS_VIS_ADC_COUNTER          0x10
#define SI114X_ALS_VIS_ADC_GAIN             0x11
#define SI114X_ALS_VIS_ADC_MISC             0x12
    
#define SI114X_LED_REC                      0x1C
    
#define SI114X_ALS_IR_ADC_COUNTER           0x1D
#define SI114X_ALS_IR_ADC_GAIN              0x1E
#define SI114X_ALS_IR_ADC_MISC              0x1F
//  
// User Settings Define  
//  
// ADCMUX    
#define SI114X_ADCMUX_SMALL_IR              0x00
#define SI114X_ADCMUX_VISIABLE              0x02
#define SI114X_ADCMUX_LARGE_IR              0x03
#define SI114X_ADCMUX_NO                    0x06
#define SI114X_ADCMUX_GND                   0x25
#define SI114X_ADCMUX_TEMPERATURE           0x65
#define SI114X_ADCMUX_VDD                   0x75
// LED SEL   
#define SI114X_PSLED12_SELECT_PS1_NONE      0x00
#define SI114X_PSLED12_SELECT_PS1_LED1      0x01
#define SI114X_PSLED12_SELECT_PS1_LED2      0x02
#define SI114X_PSLED12_SELECT_PS1_LED3      0x04
#define SI114X_PSLED12_SELECT_PS2_NONE      0x00
#define SI114X_PSLED12_SELECT_PS2_LED1      0x10
#define SI114X_PSLED12_SELECT_PS2_LED2      0x20
#define SI114X_PSLED12_SELECT_PS2_LED3      0x40
#define SI114X_PSLED3_SELECT_PS2_NONE       0x00
#define SI114X_PSLED3_SELECT_PS2_LED1       0x10
#define SI114X_PSLED3_SELECT_PS2_LED2       0x20
#define SI114X_PSLED3_SELECT_PS2_LED3       0x40
// ADC GAIN DIV  
#define SI114X_ADC_GAIN_DIV1                0x00
#define SI114X_ADC_GAIN_DIV2                0x01
#define SI114X_ADC_GAIN_DIV4                0x02
#define SI114X_ADC_GAIN_DIV8                0x03
#define SI114X_ADC_GAIN_DIV16               0x04
#define SI114X_ADC_GAIN_DIV32               0x05
// LED CURRENT   
#define SI114X_LED_CURRENT_5MA              0x01
#define SI114X_LED_CURRENT_11MA             0x02
#define SI114X_LED_CURRENT_22MA             0x03
#define SI114X_LED_CURRENT_45MA             0x04
// Recovery period the  ADC takes before making a PS measurement
#define SI114X_ADC_COUNTER_1ADCCLK          0x00
#define SI114X_ADC_COUNTER_7ADCCLK          0x01
#define SI114X_ADC_COUNTER_15ADCCLK         0x02
#define SI114X_ADC_COUNTER_31ADCCLK         0x03
#define SI114X_ADC_COUNTER_63ADCCLK         0x04
#define SI114X_ADC_COUNTER_127ADCCLK        0x05
#define SI114X_ADC_COUNTER_255ADCCLK        0x06
#define SI114X_ADC_COUNTER_511ADCCLK        0x07
// ADC MISC
#define SI114X_ADC_MISC_LOWRANGE            0x00
#define SI114X_ADC_MISC_HIGHRANGE           0x20
#define SI114X_ADC_MISC_ADC_NORMALPROXIMITY 0x00
#define SI114X_ADC_MISC_ADC_RAWADC          0x04
// INT OE
#define SI114X_INT_CFG_INTOE                0x01
// IRQ ENABLE
#define SI114X_IRQEN_ALS                    0x01
#define SI114X_IRQEN_PS1                    0x04
#define SI114X_IRQEN_PS2                    0x08
#define SI114X_IRQEN_PS3                    0x10

#define SI114X_ADDR                         0x60

class SunlightSensor : public Component {
    public:
        SunlightSensor();
        void run(serviceType);     // update value   
        void setup();              // initialize the detector
        unsigned int readVisible();
        unsigned int readIR();
        unsigned int readProximity(byte psn);
        float readUV();
        
    private:
        byte readParamData(byte reg);
        byte writeParamData(byte reg, byte value);
        void writeByte(byte reg, byte value);
        byte readByte(byte reg);
        unsigned int readUInt(byte reg);
        byte m_state;
};

#endif // _SUNLIGHT_SENSOR_H_
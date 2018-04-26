/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  LoRaWAN d|bonair application with DBoard Library (v1.0.0)

*/

// DBoard Components
#include <LightSensor.h>
#include <AirQualitySensor.h>
#include <DHTSensor.h>
#include <PIRMotionSensor.h>
#include <CO2Sensor.h>
#include <ButtonSensor.h>
#include <PulseLed.h>
#include <Buzzer.h>
#include <MDotLoRa.h>
#include <Lcd.h>
#include <Timer.h>

// Specifics Definitions
#include <LoRa.h>
#include <Sensor.h>

//////////////  Arduino UNO Pinout  \\\\\\\\\\\\\\\

// Analog Pins
//
LightSensor         light(A0);      // pin A0
AirQualitySensor    air(A1);        // pin A1
DHTSensor           dht(A2, DHT22); // pin A2
PIRMotionSensor     pir(A3);        // pin A3
// reserved for SDA (i2C)           // pin A4
// reserved for SCL (i2C)           // pin A5
// ATmega-328P if connected         // pin A6
// ATmega-328P if connected         // pin A7

// Digital Pins
//
// reserved for Serial/RX           // pin D0
// reserved for Serial/TX           // pin D1
CO2Sensor           co2(2, 3);      // pin D2
// reserved for tone() function     // pin D3
ButtonSensor        button(4);      // pin D4
PulseLed            led(5);         // pin D5
Buzzer              piezo(6);       // pin D6   
// free to use                      // pin D7 
MDotLoRa            mDot;           // pin D8
// reserved for MDot LoRa with D8   // pin D9
// free to use                      // pin D10
// reserved for tone() function     // pin D11
// free to use                      // pin D12
// free to use                      // pin D13

// I2C Ports
//
Lcd                 lcd;            // pin SDA/SCL

// Software-only component
//
Timer               timer;  
Timer               lcd_backlight_timer;

//      
//________________END OF PINOUT____________________

// LCD HD44780 Custom Character reservations //
//
#define CHAR_LORA       0            // CGRAM 0
#define CHAR_TEMP_UP    1            // CGRAM 1
#define CHAR_TEMP_DOWN  2            // CGRAM 2
#define CHAR_CO2_1      3            // CGRAM 3
#define CHAR_CO2_2      4            // CGRAM 4
#define CHAR_HUMIDITY   5            // CGRAM 5
#define CHAR_AIR        6            // CGRAM 6
// free to use                       // CGRAM 7
//_____________________________________________

// Blueish LCD screen
//
#define LCD_COLOR_RED   0
#define LCD_COLOR_GREEN 0
#define LCD_COLOR_BLUE  255

// LCD Screen definition 
//
#define LCD_SCREEN_MAIN 0
#define LCD_SCREEN_AIR  1
#define LCD_SCREEN_LORA 2
#define LCD_SCREEN_NBR  3

// Global variables
//
bool lcd_backlight;
byte lcd_screen;

// Presence counter
//
int motion;

void onEventAir(eventType e) {
    switch (e) {
        case EVENT_AIR_POLLUTION_HIGH:
            lcd.createChar(CHAR_AIR, air_4);
            piezo.play(NOTE_A7, 2000);
            Serial.print(F("[ Main ] Pollution is high ! "));
            break;
        case EVENT_AIR_POLLUTION_MEDIUM:
            lcd.createChar(CHAR_AIR, air_3);
            piezo.play(NOTE_A7, 1000);
            Serial.print(F("[ Main ] Pollution is medium ! "));
            break;
        case EVENT_AIR_POLLUTION_LOW:
            lcd.createChar(CHAR_AIR, air_2);
            piezo.play(NOTE_A7, 500);
            //Serial.print("Pollution is low ! ");
            break;
        default:
            lcd.createChar(CHAR_AIR, air_1);
    }
    //Serial.println(air.value());
} // onEventAir

void onEventDht(eventType e) {
    switch (e) {    
        case EVENT_DHT_READY:
            lcd.setCursor(2, 1);
            lcd.print(dht.temperature());
            lcd.setCursor(6, 1);
            lcd.write(0xDF);
            lcd.print("C ");
            lcd.write(CHAR_HUMIDITY);
            lcd.setCursor(10, 1);
            lcd.print(" ");
            lcd.print(dht.humidity());
            lcd.setCursor(15, 1);
            lcd.print("% ");
            break;
    }
} // onEventDht

void onEventCO2(eventType e) {
    switch (e) {
        case EVENT_CO2_DATA:
            lcd.setCursor(14, 0);
            lcd.print(" ");
            lcd.setCursor(4, 0);
            lcd.write(CHAR_CO2_1); 
            lcd.write(CHAR_CO2_2);
            lcd.setCursor(7, 0);
            lcd.print(co2.concentration());
            lcd.print(" ppm");
            if (co2.isReady() && (co2.concentration() > 1000)) {
                piezo.play(NOTE_C8, 500);
                Serial.print(F("[ Main ] High CO2 concentration: "));
                Serial.print(co2.concentration());
                Serial.println(F(" ppm"));
            } else if (co2.isReady() && (co2.concentration() > 1500)) {
                piezo.play(NOTE_C8, 2000);
                Serial.print(F("[ Main ] Very high CO2 concentration: "));
                Serial.print(co2.concentration());
                Serial.println(F(" ppm"));
            }
    }
} // onEventCO2

void onEventButton(eventType e) {
    switch (e) {
        case EVENT_BTN_SHORT_PRESS:
        case EVENT_BTN_LONG_PRESS:
            lcd_backlight_timer.reset();  // prevent the timer from elapsing
            lcd_backlight = true;
            lcd.fadeTo(5, LCD_COLOR_RED, LCD_COLOR_GREEN, LCD_COLOR_BLUE);
            break;
    }
} // onEventButton

void onEventTimer(eventType e) {
    //
    // DBoard LoRa Frame (DBLF) Format
    //
    // 0xDB (start-stop) | 0x01 (T° type) | T° High-byte | T° Low-byte | 0x02 (Angle type) | Angle High-byte | Angle Low-byte | >
    // > 0x03 (Light type) | Light High-byte | Light Low-byte | ... | 0xDB (start-stop)
    //
    #define DBLF_TYPE_TEMPERATURE   0x01
    #define DBLF_TYPE_ANGLE         0x02
    #define DBLF_TYPE_LIGHT         0x03
    #define DBLF_TYPE_HUMIDITY      0x04
    #define DBLF_TYPE_CO2           0x05
    #define DBLF_TYPE_AIR_QUALITY   0x06
    #define DBLF_TYPE_MOTION        0x07
    #define DBLF_TYPE_RSSI          0x08
    #define DBLF_TYPE_SNR           0x09

    #define DBLF_TYPE_START_STOP    0xDB

    byte frame[26]; // /!\ you should increase the size according to frame content 
    byte frameIndex = 0;

    if ((e == EVENT_TMR_ALARM) && mDot.isReady()) {
        int tempInt = dht.temperature() * 16; // compress float (IEEE-754) 4-bytes T° into 2-bytes representation.
        int humiInt = dht.humidity() * 16;

        frame[frameIndex++] = DBLF_TYPE_START_STOP;
        frame[frameIndex++] = DBLF_TYPE_TEMPERATURE;
        frame[frameIndex++] = highByte(tempInt);
        frame[frameIndex++] = lowByte(tempInt);
        frame[frameIndex++] = DBLF_TYPE_HUMIDITY;
        frame[frameIndex++] = highByte(humiInt);
        frame[frameIndex++] = lowByte(humiInt);
        frame[frameIndex++] = DBLF_TYPE_LIGHT;
        frame[frameIndex++] = light.value();
        if (co2.isReady()) {
            frame[frameIndex++] = DBLF_TYPE_CO2;
            frame[frameIndex++] = highByte(co2.concentration());
            frame[frameIndex++] = lowByte(co2.concentration());
        }
        if (air.isReady()) {
            frame[frameIndex++] = DBLF_TYPE_AIR_QUALITY;
            frame[frameIndex++] = highByte(air.value());
            frame[frameIndex++] = lowByte(air.value());
        }
        frame[frameIndex++] = DBLF_TYPE_MOTION;
        frame[frameIndex++] = motion;
        motion = 0;
        if (mDot.rssi() != 0.0) {
            int rssiInt = mDot.rssi() * 16;
            frame[frameIndex++] = DBLF_TYPE_RSSI;
            frame[frameIndex++] = highByte(rssiInt);
            frame[frameIndex++] = lowByte(rssiInt);
        }
        if (mDot.snr() != 0.0) {
            int snrInt = mDot.snr() * 16;
            frame[frameIndex++] = DBLF_TYPE_SNR;
            frame[frameIndex++] = highByte(snrInt);
            frame[frameIndex++] = lowByte(snrInt);
        }
        frame[frameIndex++] = DBLF_TYPE_START_STOP;

        if (!mDot.sendb(frame, frameIndex)) Serial.println(F("[ Main ] LoRaWAN Sending error !"));
    }
} // onEventTimer

void onEventLoRa(eventType e) {
    switch (e) {
        case EVENT_MDOT_INIT:
            // Clear LoRa Character locations
            lcd.createChar(CHAR_LORA, loraEmpty);
            lcd.setCursor(15, 0);
            lcd.write((unsigned char)CHAR_LORA); // 0 is ambiguous for GCC 
            break;
        case EVENT_MDOT_NOT_FOUND:
            lcd.createChar(CHAR_LORA, loraNotFound);
            break;
        case EVENT_MDOT_FOUND:
            lcd.createChar(CHAR_LORA, loraFound);
            break;
        case EVENT_MDOT_CONNECT_TRYING:
            lcd.animateChar(CHAR_LORA, loraConnectingSize, loraConnecting, 100, true);
            break;
        case EVENT_MDOT_CONNECT_SUCCESS:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraConnected);
            break;
        case EVENT_MDOT_CONNECT_FAILURE:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraConnectError);
            break;
        case EVENT_MDOT_CONNECT_WAITING:
            lcd.animateChar(CHAR_LORA, loraConnectWaitSize, loraConnectWait, 1000, true);
            break;
        case EVENT_MDOT_SEND_READY:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraSendReady);
            break;
        case EVENT_MDOT_SEND_ATTEMPT | EVENT_MDOT_SEND_ON_AIR:
            lcd.animateChar(CHAR_LORA, loraSendOnAirSize, loraSendOnAir, 100, true);
            break;
        case EVENT_MDOT_SEND_DELAYED:
            lcd.animateChar(CHAR_LORA, loraSendWaitSize, loraSendWait, 1000, true);
            break;
        case EVENT_MDOT_SEND_DISCONNECT | EVENT_MDOT_SEND_TOO_BIG:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraSendError);
            break;
        case EVENT_MDOT_LORA_RSSI_RECV:
            break;
        case EVENT_MDOT_LORA_SNR_RECV:
            break;
        default:
            //Serial.print("Unhandle Mdot/LoRa event: ");
            //Serial.println(e);
            break;
    }
} // onEventLoRa

// Handle LCD backlight
//
void onEventPir(eventType e) {
    switch (e) {
        case EVENT_PIR_MOVING:
            motion++;
            lcd_backlight_timer.reset();  // prevent the timer from elapsing
            lcd_backlight = true;
            lcd.fadeTo(5, LCD_COLOR_RED, LCD_COLOR_GREEN, LCD_COLOR_BLUE);
            break;
    }
} // onEventPir

void onTimeoutLcdBacklightTimer(eventType e) {
    lcd.fadeTo(30, 0, 0, 0);
    lcd_backlight = false;
} // onTimeoutWatchdogLcd

void setup() {

    // Plug & setup all components
    light.plug();
    air.plug(onEventAir);
    dht.plug(onEventDht);
    pir.plug(onEventPir);
    co2.plug(onEventCO2);
    button.plug(onEventButton);
    led.plug();
    mDot.plug(onEventLoRa);
    lcd.plug();
    timer.plug(onEventTimer);
    lcd_backlight_timer.plug(onTimeoutLcdBacklightTimer);

    // Operation on components
    motion = 0;
    led.pulse(0.5);
    timer.set(300000, true); // send a message every 5 mins

    lcd_backlight_timer.set(5000);
    lcd_backlight = true;
    lcd_screen = LCD_SCREEN_MAIN;
    lcd.setRGB(0, 0, 255);


    lcd.createChar(CHAR_CO2_1, co2_1);
    lcd.createChar(CHAR_CO2_2, co2_2);
    lcd.createChar(CHAR_HUMIDITY, drop);
    lcd.createChar(CHAR_TEMP_UP, tempUp);
    lcd.createChar(CHAR_TEMP_DOWN, tempDown);
    lcd.setCursor(0, 0);
    lcd.write(CHAR_TEMP_UP); // 0 is ambiguous for GCC 
    lcd.setCursor(0, 1);
    lcd.write(CHAR_TEMP_DOWN);
    lcd.createChar(CHAR_AIR, air_0);
    lcd.setCursor(2, 0);
    lcd.write(CHAR_AIR);
} // setup

void loop() {
    Component::loop();
} // loop
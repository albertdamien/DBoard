/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  LoRaWAN Demo with MultiTECH mtDot module and DBoard Library (v1.0.0)

*/

// DBoard Components
#include <LightSensor.h>
#include <TemperatureSensor.h>
#include <SoundSensor.h>
#include <RotaryAngleSensor.h>
#include <TouchSensor.h>
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
TemperatureSensor   temp(A1);       // pin A1
SoundSensor         sound(A2);      // pin A2
RotaryAngleSensor   knob(A3);       // pin A3
// reserved for SDA (i2C)           // pin A4
// reserved for SCL (i2C)           // pin A5
// ATmega-328P if connected         // pin A6
// ATmega-328P if connected         // pin A7

// Digital Pins
//
// reserved for Serial/RX           // pin D0
// reserved for Serial/TX           // pin D1
TouchSensor         touch(2);       // pin D2
// reserved for tone() function     // pin D3
ButtonSensor        button(4);      // pin D4
PulseLed            blueLed(5);     // pin D5
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
Timer               watchdogLcd;

// Global variables
//
bool                awake;

//      
//________________END OF PINOUT____________________

// LCD HD44780 Custom Character reservations //
//
#define CHAR_LORA       0            // CGRAM 0
#define CHAR_TEMP_UP    2            // CGRAM 1
#define CHAR_TEMP_DOWN  3            // CGRAM 2
// free to use                       // CGRAM 3
// free to use                       // CGRAM 4
// free to use                       // CGRAM 5
// free to use                       // CGRAM 6
// free to use                       // CGRAM 7
//_____________________________________________

void onEventLight(eventType e) {
    switch (e) {
        case EVENT_DAY:
            Serial.println(F("Il fait jour !"));
            break;
        case EVENT_NIGHT:
            Serial.println(F("Il fait nuit !"));
            break;
    }
} // onEventLight

void onEventTemp(eventType e) {
    switch (e) {    
        case EVENT_CHANGE:
            lcd.setCursor(2, 1);
            lcd.print(temp.value());
            lcd.setCursor(6, 1);
            lcd.write(0xDF);
            lcd.print("C ");
            break;
    }
} // onEventTemp

void onEventKnob(eventType e) {
    if (e == EVENT_KNOB) {
        watchdogLcd.reset();  // prevent the timer from elapsing
        if (awake) {
            lcd.setCursor(10, 1);
            lcd.write(0xE0);
            lcd.print(':');
            lcd.print("    ");
            lcd.setCursor(12, 1);
            lcd.print(knob.value());
        } else {
            for (int i = 0; i < 255; i++) {
                    lcd.setRGB(0, 0, i);
            }
            awake = true;
        }
    }
} // onEventKnob

void onEventTouch(eventType e) {
    switch (e) {
        case EVENT_SHORT_TOUCH:
            piezo.play(NOTE_A3, 50);
            break;
        case EVENT_LONG_TOUCH:
            piezo.play(NOTE_C3, 50);
            break;
    }
} // onEventTouch

void onEventButton(eventType e) {
    switch (e) {
        case EVENT_SHORT_PRESS:
            piezo.play(NOTE_C1, 100);
            break;
        case EVENT_LONG_PRESS:
            piezo.play(NOTE_C3, 200);
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
    #define frameSize 11
    byte frame[frameSize];

    if ((e == EVENT_ALARM) && mDot.isReady()) {
        int tempInt = temp.value() * 16; // compress float (IEEE-754) 4-bytes T° into 2-bytes representation.
        int angleInt = knob.value();
        int lightInt = light.value();

        frame[0] = 0xDB;
        frame[1] = 0x01;
        frame[2] = highByte(tempInt);
        frame[3] = lowByte(tempInt);
        frame[4] = 0x02;
        frame[5] = highByte(angleInt);
        frame[6] = lowByte(angleInt);
        frame[7] = 0x03;
        frame[8] = highByte(lightInt);
        frame[9] = lowByte(lightInt);
        frame[10] = 0xDB;

        mDot.sendb(frame, frameSize);
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
        case EVENT_CONNECT_TRYING:
            lcd.animateChar(CHAR_LORA, loraConnectingSize, loraConnecting, 100, true);
            break;
        case EVENT_CONNECT_SUCCESS:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraConnected);
            break;
        case EVENT_CONNECT_FAILURE:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraConnectError);
            break;
        case EVENT_CONNECT_WAITING:
            lcd.animateChar(CHAR_LORA, loraConnectWaitSize, loraConnectWait, 1000, true);
            break;
        case EVENT_SEND_READY:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraSendReady);
            break;
        case EVENT_SEND_ATTEMPT:
            lcd.animateChar(CHAR_LORA, loraSendingSize, loraSending, 100, true);
            break;
        case EVENT_SEND_DELAYED:
            lcd.animateChar(CHAR_LORA, loraSendWaitSize, loraSendWait, 1000, true);
            break;
        case EVENT_SEND_DISCONNECT | EVENT_SEND_TOO_BIG:
            lcd.noAnimateChar(CHAR_LORA);
            lcd.createChar(CHAR_LORA, loraSendError);
            break;
        case EVENT_SEND_ON_AIR:
            lcd.animateChar(CHAR_LORA, loraSendOnAirSize, loraSendOnAir, 100, true);
            break;
        default:
            //Serial.print("Unhandle Mdot/LoRa event: ");
            //Serial.println(e);
            break;
    }
} // onEventLoRa

void onTimeoutWatchdogLcd(eventType e) {
    for (int i = 255; i >= 0; i--) {
        lcd.setRGB(0, 0, i);
    }
    awake = false;
} // onTimeoutWatchdogLcd

void setup() {

    // Plug & setup all components
    light.plug(onEventLight);
    temp.plug(onEventTemp);
    sound.plug(); // TODO
    knob.plug(onEventKnob);
    touch.plug(onEventTouch);
    button.plug(onEventButton);
    blueLed.plug();
    mDot.plug(onEventLoRa);
    lcd.plug();
    timer.plug(onEventTimer);
    watchdogLcd.plug(onTimeoutWatchdogLcd);

    // Operation on components
    awake = true;
    blueLed.pulse(1);
    temp.tune(30, 18);
    timer.set(120000, true);
    watchdogLcd.set(5000);

    lcd.setRGB(0, 0, 255);
    lcd.setCursor(2, 0);
    lcd.print(F("LoRa/DBoard"));

    lcd.createChar(CHAR_TEMP_UP, tempUp);
    lcd.createChar(CHAR_TEMP_DOWN, tempDown);
    lcd.setCursor(0, 0);
    lcd.write(CHAR_TEMP_UP); // 0 is ambiguous for GCC 
    lcd.setCursor(0, 1);
    lcd.write(CHAR_TEMP_DOWN);

} // setup

void loop() {
    Component::loop();
} // loop
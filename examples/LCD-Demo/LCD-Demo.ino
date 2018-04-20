/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  LCD Demo with DBoard Library (v1.0.0)

*/

// DBoard Components
#include <LightSensor.h>
#include <TemperatureSensor.h>
#include <RotaryAngleSensor.h>
#include <ButtonSensor.h>
#include <PulseLed.h>
#include <Buzzer.h>
#include <Lcd.h>
#include <Timer.h>

// Specifics Definitions
#include <Sensor.h>

//////////////  Arduino UNO Pinout  \\\\\\\\\\\\\\\

// Analog Pins
//
LightSensor         light(A0);      // pin A0
TemperatureSensor   temp(A1);       // pin A1
// free to use                      // pin A2
RotaryAngleSensor   knob(A3);       // pin A3
// reserved for SDA (i2C)           // pin A4
// reserved for SCL (i2C)           // pin A5
// ATmega-328P if connected         // pin A6
// ATmega-328P if connected         // pin A7

// Digital Pins
//
// reserved for Serial/RX           // pin D0
// reserved for Serial/TX           // pin D1
// free to use                      // pin D2
// reserved for tone() function     // pin D3
ButtonSensor        button(4);      // pin D4
PulseLed            redLed(5);      // pin D5
Buzzer              piezo(6);       // pin D6   
// free to use                      // pin D7 
// free to use                      // pin D8
// free to use                      // pin D9
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
//      
//________________END OF PINOUT____________________

// LCD HD44780 Custom Character reservations //
//
#define CHAR_LIGHT      0            // CGRAM 0
#define CHAR_TEMP_UP    1            // CGRAM 1
#define CHAR_TEMP_DOWN  2            // CGRAM 2
// free to use                       // CGRAM 3
// free to use                       // CGRAM 4
// free to use                       // CGRAM 5
// free to use                       // CGRAM 6
// free to use                       // CGRAM 7
//_____________________________________________

// Global variables
//
bool                awake;

// Event handlers 
//
void onEventLight(eventType e) {
    switch (e) {
        case EVENT_DAY:
            Serial.println(F("Il fait jour !"));
            break;
        case EVENT_NIGHT:
            Serial.println(F("Il fait nuit !"));
            break;
        case EVENT_CHANGE:
            lcd.setCursor(10, 0);
            lcd.write((unsigned char)CHAR_LIGHT);
            lcd.print(':');
            lcd.print("    ");
            lcd.setCursor(12, 0);
            lcd.print(light.value());
            lcd.print("%");
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
            lcd.fadeTo(5, 255, 0, 0);
            awake = true;
        }
    }
} // onEventKnob

void onEventButton(eventType e) {
    int i;
    switch (e) {
        case EVENT_SHORT_PRESS:
            piezo.play(NOTE_C2, 200);
            break;
        case EVENT_LONG_PRESS:
            piezo.play(NOTE_C3, 200);
            break;
    }
} // onEventButton

void onEventTimer(eventType e) {
    if ((e == EVENT_ALARM)) {
        //piezo.play(NOTE_E3, 100);
        Serial.println("Alarme !");
    }
} // onEventTimer

void onTimeoutWatchdogLcd(eventType e) {
    lcd.fadeTo(30, 0, 0, 0);
    awake = false;
} // onTimeoutWatchdogLcd

// Main Arduino setup() function
//
void setup() {

    // Plug & setup all components
    light.plug(onEventLight);
    temp.plug(onEventTemp);
    knob.plug(onEventKnob);
    button.plug(onEventButton);
    redLed.plug();
    lcd.plug();
    timer.plug(onEventTimer);
    watchdogLcd.plug(onTimeoutWatchdogLcd);

    // Operation on components
    awake = true;
    redLed.pulse(2);
    temp.tune(30, 18);
    timer.set(300000, true);
    watchdogLcd.set(5000);

    lcd.setRGB(255, 0, 0);
    lcd.setCursor(2, 0);
    lcd.print(F("DBoard"));

    lcd.createChar(CHAR_LIGHT, lux);
    lcd.createChar(CHAR_TEMP_UP, tempUp);
    lcd.createChar(CHAR_TEMP_DOWN, tempDown);
    lcd.setCursor(0, 0);
    lcd.write(CHAR_TEMP_UP); // 0 is ambiguous for GCC 
    lcd.setCursor(0, 1);
    lcd.write(CHAR_TEMP_DOWN);

} // setup

// Main Arduino loop() function
//
void loop() {
    Component::loop();
} // loop
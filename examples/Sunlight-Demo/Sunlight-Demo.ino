/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Sunlight Sensor Demo with DBoard Library (v1.0.0)

*/

#include <SunlightSensor.h>
#include <Timer.h>

SunlightSensor sun;
Timer          tmr;

void onEventSun(eventType e) {
    switch (e) {
        case EVENT_SUNLIGHT_NOT_READY:
            Serial.println("Sensor is not ready");
            break;
        case EVENT_SUNLIGHT_READY:
            Serial.println("Sensor is ready !");
            tmr.set(2000, true);
            break;
    }
} 

void onEventTimer(eventType e) {
    Serial.print("//--------------------------------------//\r\n");
    Serial.print("Vis: "); Serial.println(sun.readVisible());
    Serial.print("IR: "); Serial.println(sun.readIR());
    Serial.print("UV: ");  Serial.println(sun.readUV());
} 

void setup() {
    sun.plug(onEventSun);
    tmr.plug(onEventTimer);
} 

void loop() {
    Component::loop();
}
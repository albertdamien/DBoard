/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Sunlight Sensor Demo with DBoard Library (v1.0.0)

*/

#include <MoistureSensor.h>
#include <Timer.h>

MoistureSensor moisture(A0);

void onEventMoisture(eventType e) {
    switch (e) {
        case EVENT_MOISTURE_DRY:
            Serial.println("It's dry !");
            break;
        case EVENT_MOISTURE_HUMID:
            Serial.println("It's humid !");
            break;
        case EVENT_MOISTURE_WATER:
            Serial.println("It's water !");
            break;
    }
    Serial.println(moisture.value());
} 

void setup() {
    moisture.plug(onEventMoisture);
    moisture.probe(5000);
} 

void loop() {
    Component::loop();
}
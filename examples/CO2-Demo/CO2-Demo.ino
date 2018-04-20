/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple CO2 Demo with DBoard Library (v1.0.0)

*/

#include <CO2Sensor.h>

CO2Sensor co2(2, 3); // pin D2 & D3

void onEventCo2(eventType e) {
    switch (e) {
        case EVENT_WARMING:
            Serial.print("Warming up (");
            Serial.print(co2.temperature());
            Serial.println(" °C)");
            break;
        case EVENT_READY:
            Serial.println("Sensor is ready !");
            break;
        case EVENT_DATA:
            Serial.print("Co2 concentration : ");
            Serial.print(co2.concentration());
            Serial.println(" ppm");
            break;
    }
} 

void setup() {
    co2.plug(onEventCo2);
} 

void loop() {
    Component::loop();
}
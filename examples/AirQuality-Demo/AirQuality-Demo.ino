/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Air Quality Demo with DBoard Library (v1.0.0)

*/

#include <AirQualitySensor.h>

AirQualitySensor air(A1); 

void onEventAir(eventType e) {
    switch (e) {
        case EVENT_READY:
            Serial.println("Air Quality Sensor is ready");
            break;
        case EVENT_WARMUP:
            Serial.println("Air Quality Sensor is warming up ...");
            break;
        case EVENT_ERROR:
            Serial.println("Air Quality Sensor has a problem !");
            break;
        case EVENT_POLLUTION_HIGH:
            Serial.print("Pollution is high ! ");
            break;
        case EVENT_POLLUTION_MEDIUM:
            Serial.print("Pollution is medium ! ");
            break;
        case EVENT_POLLUTION_LOW:
            Serial.print("Pollution is low ! ");
            break;
        case EVENT_FRESH_AIR:
            Serial.print("Fresh air ! ");
            break;
    }
    Serial.println(air.value());
} 

void setup() {
    air.plug(onEventAir);
} 

void loop() {
    Component::loop();
}
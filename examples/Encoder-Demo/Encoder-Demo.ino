/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Encoder Demo with DBoard Library (v1.0.0)

*/

#include <Encoder.h>

Encoder encoder(7, 8); // pin D7 & D8

void onEventEncoder(eventType e) {
    switch (e) {
        case EVENT_ENC_CW:
            Serial.println("Clockwise pulse");
            break;
        case EVENT_ENC_CCW:
            Serial.println("Counterclockwise pulse");
            break;
    }
} 

void setup() {
    encoder.plug(onEventEncoder);
} 

void loop() {
    Component::loop();
}
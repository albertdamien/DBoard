/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Blinking-Led Demo with DBoard Library (v1.0.0)

*/

#include <PIRMotionSensor.h>

PIRMotionSensor pir(A3);

void onEvent(eventType e) {
    Serial.print("Someone is here @");
    Serial.print(millis());
    Serial.println();
} 

void setup() {
    pir.plug(onEvent);
} 

void loop() {
    Component::loop();
}
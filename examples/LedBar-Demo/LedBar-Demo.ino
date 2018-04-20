/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Led Bar Demo with DBoard Library (v1.0.0)

*/

#include <LedBar.h>
#include <RotaryAngleSensor.h>

LedBar              bar(6, 7, true); // pin D6
RotaryAngleSensor   knob(A3);        // pin A3

void onEventKnob(eventType e) {
    float level;
    if (e == EVENT_KNOB) {
        level = (float)(knob.value() * 10) / 1022;
        bar.setLevel(level);
    }
} // onEventKnob

void setup() {
    bar.plug();
    knob.plug(onEventKnob);
} 

void loop() {
    Component::loop();
}
/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Relay Demo with DBoard Library (v1.0.0)

*/

#include <ButtonSensor.h>
#include <Relay.h>

ButtonSensor    button(4); // pin D4
Relay           relay(2);  // pin D2

void onEventButton(eventType e) {
    switch (e) {
        case EVENT_BTN_SHORT_PRESS:
            relay.close();
            break;
        case EVENT_BTN_LONG_PRESS:
            relay.open();
            break;
    }
} 

void setup() {
    button.plug(onEventButton);
    relay.plug();
} 

void loop() {
    Component::loop();
}
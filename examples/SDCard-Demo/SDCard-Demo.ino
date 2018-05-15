/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Blinking-Led Demo with DBoard Library (v1.0.0)

*/

#include <ButtonSensor.h>
#include <BlinkLed.h>

ButtonSensor    button(6); // pin D4
BlinkLed        led(5);    // pin D5

void onEventButton(eventType e) {
    switch (e) {
        case EVENT_BTN_SHORT_PRESS:
            led.blink(50, 100);
            break;
        case EVENT_BTN_LONG_PRESS:
            led.off();
            break;
    }
} 

void setup() {
    button.plug(onEventButton);
    led.plug();
    led.blink(50, 100);
} 

void loop() {
    Component::loop();
}
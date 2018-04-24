/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Lcd12864 Demo with DBoard Library (v1.0.0)

*/

#include <Oled12864.h>

Oled12864 oled; 

void onEventOled(eventType e) {
    switch (e) {
        case EVENT_READY:
            oled.clearDisplay();                // clear the screen and set start position to top left corner
            oled.setNormalDisplay();            // set display to normal mode (i.e non-inverse mode)
            oled.setPageMode();                 // set addressing mode to Page Mode
            oled.setTextXY(0,0);                // set the cursor to Xth Page, Yth Column  
            oled.putString("Hello d|board !");  // print the Stringv
            oled.setTextXY(1,0);           //Set the cursor to 0th Page, 0th Column  
            oled.setHorizontalMode();
            oled.putString("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
            break;
    }
} 

void setup() {
    oled.plug(onEventOled);
} 

void loop() {
    Component::loop();
}
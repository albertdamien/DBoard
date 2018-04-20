/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple Lcd12864 Demo with DBoard Library (v1.0.0)

*/

#include <Lcd12864.h>

Lcd12864 lcd; 
extern GUI_Bitmap_t bmmn;

void setup() {
    lcd.plug();

    lcd.cleanAll(WHITE);    //Clean the screen with black or white.
    
/*
    lcd.fontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 

    lcd.charGotoXY(0,0);       //Set the start coordinate.
    lcd.print("Hello World!");  //Display "Hello World!" on coordinate of (0, 10).
    lcd.charGotoXY(0,9);       //Set the start coordinate.
    lcd.print("d|bora board");  //Display "Hello World!" on coordinate of (0, 10).
*/
    lcd.workingModeConf(ON, ON, WM_BitmapMode);

    //Display "bmTuzki" on coordinate of (30, 0).
    //Bitmap data package file generate method please refer to the 5.3 section of the "I2C_LCD User Manual".
    lcd.drawScreenAreaAt(&bmmn, 30, 0);
} 

void loop() {
    Component::loop();
}
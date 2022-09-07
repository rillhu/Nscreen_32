/*
 This test case is copied and modified from 
 "lvgl\examples\arduino\LVGL_Arduino\LVGL_Arduino.ino"
*/
#include <Arduino.h>
#include <lvgl.h>
#include "display.h"

void setup()
{
	Serial.begin(115200); /* prepare for possible serial debug */
	Serial.println("I am LVGL_Arduino");

	displaySetup();

	Serial.println("Setup done");
}

void loop()
{
	// lv_timer_handler(); /* let the GUI do its work */
	//delay( 5 );
}

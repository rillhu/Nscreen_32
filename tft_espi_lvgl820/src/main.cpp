/*
 This test case is copied and modified from 
 "lvgl\examples\arduino\LVGL_Arduino\LVGL_Arduino.ino"
*/
#include <Arduino.h>
#include <lvgl.h>
#include "display_service.h"

#include "esp_freertos_hooks.h"

display_service display; //load display service

static void lv_tick_task()
{
    lv_tick_inc(portTICK_PERIOD_MS);
}

SemaphoreHandle_t xGuiSemaphore = xSemaphoreCreateMutex();
void lv_task_main(void *pvParameters)
{
    display.setup();

    esp_register_freertos_tick_hook(lv_tick_task);
    for (;;)
    {
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            display.loop(); /* let the GUI do its work */
            xSemaphoreGive(xGuiSemaphore);
        }
        delay(5);
    }
}

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("I am LVGL_Arduino");

    // displaySetup();

    xTaskCreatePinnedToCore(lv_task_main, "lv_task_main", 1024 * 8, NULL, 5, NULL, 0);

    Serial.println("Setup done");
}

void loop()
{
    // lv_timer_handler(); /* let the GUI do its work */
    //delay( 5 );
}

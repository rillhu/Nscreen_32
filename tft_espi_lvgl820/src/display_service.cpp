
/*
VERSION "0.0.0.2" :

480x320 4.0 inch ST7796 parallel display project
connectons：
===============================================
TFT---esp32
VCC:5v
GND:GND
CS   33  // Chip select control pin
DC/RS  15  // Data Command control pin - must use a pin in the range 0-31 also named RS
RST  32  // Reset pin
WR    4  // Write strobe control pin - must use a pin in the range 0-31
RD    2
TFT_D0   12  // Must use pins in the range 0-31 for the data bus
TFT_D1   13  // so a single register write sets/clears all bits
TFT_D2   26
TFT_D3   25
TFT_D4   19
TFT_D5   18
TFT_D6   27
TFT_D7   14
===============================================

*/

#include <Arduino.h>

#include <lvgl.h>
#include <TFT_eSPI.h>
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/

#include "config.h"
#include "gt911.h"
#include "display_service.h"

#include "esp_freertos_hooks.h"
#include "esp_heap_caps.h"

#include "demos/lv_demos.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "demos/benchmark/lv_demo_benchmark.h"
#include "demos/stress/lv_demo_stress.h"
#include "demos/music/lv_demo_music.h"
#include "demos/keypad_encoder/lv_demo_keypad_encoder.h"

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
#if TOUCHPAD
tp_service tp; /*touchpad instance*/
#endif

display_service::display_service()
{
}
display_service::~display_service()
{
}

/* =============================icache functions========================= */
#ifdef TOUCHPAD
void ICACHE_FLASH_ATTR display_service::touch_setup()
{
    tp.setup();
    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
#ifdef _DEBUG_
    Serial.print(F("[INFO] Touch setup finished! \n"));
#endif
}
#endif

#if LV_USE_LOG != 0
/* Serial debugging */
void ICACHE_FLASH_ATTR my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

void ICACHE_FLASH_ATTR display_service::lv_setup()
{
    lv_init();
    tft.begin(); /* TFT init */
    /* Landscape orientation, flipped 
       0, 1, 2, 3 ==> 0°、90°、180°、270°   
    */
    tft.setRotation(ROTATION);
#if TOUCH_CALIBRATION
    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    uint16_t calData[5] = {275, 3620, 264, 3532, 1};
    tft.setTouch(calData);
#endif
    lv_color_t *buf = (lv_color_t *)heap_caps_malloc(screenHeight * 100, MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenHeight * 100);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    if (ROTATION == 0)
    {
        disp_drv.hor_res = screenHeight;
        disp_drv.ver_res = screenWidth;
    }
    else
    {
        disp_drv.hor_res = screenWidth;
        disp_drv.ver_res = screenHeight;
    }

    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    lv_demo_widgets();

#ifdef _DEBUG_
    Serial.print(F("[INFO] Display GUI setup finished! \n"));
#endif
}

void ICACHE_FLASH_ATTR display_service::setup()
{
    lv_setup();

#ifdef TOUCHPAD
    touch_setup();
#endif

    lv_main();

} // end display service setup

/* =========================end icache functions======================== */

/* =========================== iram functions=========================== */
#ifdef TOUCHPAD
void IRAM_ATTR display_service::my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    bool touched = tp.get_touch(50);
    if (tp.get_xy.touch == 0)
    {
        return;
    }

    if (tp.get_xy.x > screenHeight || tp.get_xy.y > screenWidth)
    {
#ifdef _DEBUG_
        Serial.printf("[INFO][DISPLAY][TOUCH][ERR]Y or y outside of expected parameters.. X=%d, Y=%d\n", tp.get_xy.x, tp.get_xy.y);
#endif
    }
    else
    {

        data->state = (tp.get_xy.touch == true) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        // data->state = LV_INDEV_STATE_PR;

        /*Save the state and save the pressed coordinate*/
        //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);
        if (data->state == LV_INDEV_STATE_PR)
        {
            if (ROTATION == 0)
            {
                data->point.x = tp.get_xy.x;
                data->point.y = tp.get_xy.y;
            }
            else
            {
                /* ROTATION==1 */
                data->point.x = tp.get_xy.y;
                data->point.y = 319 - tp.get_xy.x;
            }
#ifdef XXXXXXXX //_DEBUG_
            Serial.printf("[INFO][TP] X is %d \n", tp.get_xy.x);
            Serial.printf("[INFO][TP] Y is %d \n", tp.get_xy.y);
            Serial.printf("[INFO][TP] Touch status is %d \n", tp.get_xy.touch);
            Serial.printf("[INFO][UPTIME] %ld \n", millis());
            Serial.printf("[INFO][MEM] FREE memory %d \n", ESP.getFreeHeap());
#endif
        }
    }

    return; /*Return `false` because we are not buffering and no more data to read*/
}
#endif

void IRAM_ATTR display_service::lv_main()
{

#ifdef _DEBUG_
    Serial.print(F("[INFO] LV GUI started.\n"));
#endif
    // header_create();
    // body_create();
}

void IRAM_ATTR display_service::loop()
{
    lv_timer_handler(); /* let the GUI do its work */
} //end loop

/* Display flushing */
void IRAM_ATTR display_service::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.startWrite();
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/* ===========================end iram functions=========================== */

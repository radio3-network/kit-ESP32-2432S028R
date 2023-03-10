/////////////////////////////////////////////////////////////////
/*
  Example of LVGL together with LovyanGFX
  for the ESP32 WROOM board ESP-2432S028.

This board is provided by Sunton: https://www.aliexpress.com/store/1100192306
at very affordable prices (~15 EUR in 2023). This source code file
provides an example of LVGL working on the board.

Contains as hardware:
+ ESP-WROOM-32
+ TF card reader
+ TFT display with touch screen
+ LDR (light sensor)

Code is based on the tutorial from ThatProject
+ https://www.youtube.com/watch?v=IPCvQ4o_WP8
+ https://github.com/0015/ThatProject/tree/master/ESP32_LVGL/LVGL8/0_ILI9488_LovyanGFX

Other relevant repositories
+ https://github.com/OttoMeister/ARDUINO_ESP32-2432S028R
+ https://github.com/rzeldent/esp32-smartdisplay
+ https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/


There exist more models from Sunton. The esp32-smartdisplay
library from the link above maintains support for them.

## Installing
+ Install Arduino, include support for ESP32
+ Install the "LovyanGFX" and "LVGL" libraries
+ copy lvgl-conf.h file to the arduino library folder
++ typically this folder is ~/Arduino/libraries/
+ Compile and upload the firmware

Versions used:
+ lvgl 8.3.5
+ LovyanGFX 1.1.5


License: Apache-2.0
Copyright (c) radio3.network

More projects at https://github.com/radio3-network/
  
*/
/////////////////////////////////////////////////////////////////

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"
#include <lvgl.h>

/* Change to your screen resolution */
static const uint32_t screenWidth  = 320;
static const uint32_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}

void setup()
{
   Serial.begin(115200);

   tft.begin();        
   tft.setRotation(1);
   tft.setBrightness(255);
   uint16_t calData[] = {3749, 3619, 3737, 207, 361, 3595, 267, 221};
   tft.setTouchCalibrate(calData);

   lv_init();
   lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

   /*Initialize the display*/
   static lv_disp_drv_t disp_drv;
   lv_disp_drv_init(&disp_drv);

   /*Change the following line to your display resolution*/
   disp_drv.hor_res = screenWidth;
   disp_drv.ver_res = screenHeight;
   disp_drv.flush_cb = my_disp_flush;
   disp_drv.draw_buf = &draw_buf;
   lv_disp_drv_register(&disp_drv);

   /*Initialize the (dummy) input device driver*/
   static lv_indev_drv_t indev_drv;
   lv_indev_drv_init(&indev_drv);
   indev_drv.type = LV_INDEV_TYPE_POINTER;
   indev_drv.read_cb = my_touchpad_read;
   lv_indev_drv_register(&indev_drv);

   lv_example_get_started_1();
}

void loop()
{
   lv_timer_handler(); /* let the GUI do its work */
   delay( 5 );
}

static void btn_pressed(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_align(btn, LV_ALIGN_CENTER, 0,0);
    lv_obj_add_event_cb(btn, btn_pressed, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
}
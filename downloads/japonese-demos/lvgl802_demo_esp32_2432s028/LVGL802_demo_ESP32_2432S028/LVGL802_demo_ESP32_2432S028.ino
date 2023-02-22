//=====================================================================
// LVGL802 demo with ESP32_2432S028 : 2022.08.01 : macsbug
// https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
//=====================================================================
// HARD : EESP32_2432S028 : 
/// Dev environment  : Arduino IDE 1.8.19
//  Board Manager    : arduino-esp32 2.0.3-RC1
//  Board            : "ESP32 Dev Module"
//  Flash Size       : "4MB (32Mb)"
//  Partition Scheme : "No OTA (2MB APP/2MB SPIFSS)"
//  Pord             : "dev/cu.wchusbserial14240"
// Library           : lovyanGFX 
//                   : https://github.com/lovyan03/LovyanGFX
//=====================================================================
// LVGL802 demo with 16bit MRB3511 S3 Minikit : 2022.07.22 : macsbug
// https://macsbug.wordpress.com/2022/08/03/3-5%e2%80%b3-16bit-lcd-and-s3-minikit/
//---------------------------------------------------------------------
// HARD : ESP32 S3 MiniKit : Make an ESP32 S3 MiniKit
//  https://macsbug.wordpress.com/2022/08/01/make-an-esp32-s3-minikit/
//---------------------------------------------------------------------
// lvgl 8.0.2 demo : ORG : 2022.02.28 : macsbug
// https://macsbug.wordpress.com/2022/03
//---------------------------------------------------------------------
// Performance of lv_music_demo :
//  The average FPS is 9
//  23.5kB used (72 %), 30% frag.
//  62FPS, 2% CPU
//---------------------------------------------------------------------
// lv_example demo：
//   lv_demo_music
//   lv_demo_keypad_encoder
//   lv_demo_benchmark
//   lv_demo_stress
//   lv_demo_widgets
//---------------------------------------------------------------------
// lv_conf.h : Additions and Settings
// lvgl 8.0.2/lv_conf_template.h to lv_conf.h to src folder
// lvgl 8.0.2/src/lv_conf.h
//  #define LV_COLOR_DEPTH          16
//  #define LV_COLOR_16_SWAP         1
//  #define LV_DISP_DEF_REFR_PERIOD 16
//  #define LV_TICK_CUSTOM           1
//  #define LV_FONT_MONTSERRAT_12    1
//  #define LV_FONT_MONTSERRAT_14    1
//  #define LV_FONT_MONTSERRAT_16    1
//  #define LV_USE_PERF_MONITOR      1 // Show CPU usage and FPS count
//  #define LV_USE_MEM_MONITOR       1 // Show the used memory
//---------------------------------------------------------------------
// lv_demo_conf.h : Additions and Settings
// lv_examples/lv_demo_conf_template to lv_demo_conf.h
// lv_demo_conf.h
//  #if 1 
//  #define LV_USE_DEMO_WIDGETS 1
//  #define LV_DEMO_WIDGETS_SLIDESHOW 1
//  #define LV_USE_DEMO_KEYPAD_AND_ENCODER 1
//  #define LV_USE_DEMO_BENCHMARK 1
//  #define LV_USE_DEMO_STRESS 1
//  #define LV_USE_DEMO_MUSIC 1
//  #define LV_USE_DEMO_MUSIC 1
//  #define LV_DEMO_MUSIC_AUTO_PLAY 1
//
// img_ : Additions
// lv_examples/src/lv_demo_widgets/assets/
//  img_clothes.c             // add. #define LV_LVGL_H_INCLUDE_SIMPLE
//  img_demo_widgets_avatar.c // add. #define LV_LVGL_H_INCLUDE_SIMPLE
//  img_lvgl_logo.c           // add. #define LV_LVGL_H_INCLUDE_SIMPLE
//=====================================================================

#pragma GCC optimize ("Ofast")
#include <lvgl.h>
#include <lv_demo.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"
static const uint32_t screenWidth  = 320;
static const uint32_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][screenWidth * 10]; 

//=====================================================================
void setup(){
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.setBrightness(200);
  tft.setSwapBytes(true);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf,buf[0],buf[1],screenWidth*10);
  init_display();
  init_touch();
  
  lv_demo_music();            // lv_demo_music
  //lv_demo_keypad_encoder();   // lv_demo_keypad_encoder
  //lv_demo_benchmark();        // lv_demo_benchmark
  //lv_demo_stress();           // lv_demo_stress
  //lv_demo_widgets();          // lv_demo_widget *** NG ***
}

//=====================================================================
void loop(){
  lv_timer_handler(); delay(5);
}
//=====================================================================

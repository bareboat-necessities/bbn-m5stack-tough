#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>

#include "ui_init.h"
#include "ui_gestures.h"
#include "ui_theme.h"
#include "ui_screens.h"
#include "m5_rtc.h"
#include "ui_clock.h"
#include "ui_reboot.h"
#include "ui_about.h"

#include "ship_data_model.h"

#include "ui_wind.h"
#include "ui_power_victron.h"

lv_updatable_screen_t* screens[] = {
  &windScreen,
  &victronScreen,
  &clockScreen,
  &rebootScreen,
  &aboutScreen,
};

int page = 0;
int pages_count = sizeof(screens) / sizeof(screens[0]);

void next_page() {
  page++;
  if (page >= pages_count) page = 0;
  lv_scr_load(screens[page]->screen);
}

void prev_page() {
  page--;
  if (page < 0) page = pages_count - 1;
  lv_scr_load(screens[page]->screen);
}

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  init_theme();

  init_windScreen();    
  lv_scr_load(screens[page]->screen);
  init_victronScreen();    
  init_clockScreen();    
  init_rebootScreen();    
  init_aboutScreen();
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);

  if (swipe_vert_detected()) {
    toggle_ui_theme();
  }
  else if (swipe_right_detected()) {
    next_page();
  }
  else if (swipe_left_detected()) {
    prev_page();
  }
  
  update_screen(*screens[page]);
}

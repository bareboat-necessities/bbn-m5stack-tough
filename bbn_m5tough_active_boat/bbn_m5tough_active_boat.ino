#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>

#include "ui_init.h"

static void btnPowerOff_event(lv_event_t *event) {
  M5.Axp.PowerOff();
}

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  
  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_t *label = lv_label_create(btn);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label, "Power Off");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn, btnPowerOff_event, LV_EVENT_PRESSED, NULL);
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);
}

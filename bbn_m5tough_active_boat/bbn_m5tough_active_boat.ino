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

lv_updatable_screen_t scr1, scr2;

lv_updatable_screen_t* screens[] = {
  &scr1, &scr2
};

int page = 0;

static void btnPowerOff_event(lv_event_t *event) {
  M5.Axp.PowerOff();
}

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  init_theme();
  
  scr1.screen = lv_obj_create(NULL); // Creates a Screen object
  lv_obj_t *btn = lv_btn_create(scr1.screen);
  lv_obj_t *label = lv_label_create(btn);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label, "Power Off");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn, btnPowerOff_event, LV_EVENT_PRESSED, NULL);

  lv_scr_load(screens[page]->screen);
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);

  if (swipe_vert_detected()) toggle_ui_theme();
  //if (swipe_horiz_detected()) next_screen();

  update_screen(scr1);
}

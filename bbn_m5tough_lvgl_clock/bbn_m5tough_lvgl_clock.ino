#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;    // Descriptor of a display driver
static lv_indev_drv_t indev_drv;  // Descriptor of a touch driver

M5Display *tft;

void tft_lv_initialization() {
  M5.begin();
  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // Declare a buffer for 1/10 screen siz
  static lv_color_t buf2[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // second buffer is optionnal

  // Initialize `disp_buf` display buffer with the buffer(s).
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10);
  tft = &M5.Lcd;
}

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft->startWrite();
  tft->setAddrWindow(area->x1, area->y1, w, h);
  tft->pushColors((uint16_t *)&color_p->full, w * h, true);
  tft->endWrite();

  lv_disp_flush_ready(disp);
}

void init_disp_driver() {
  lv_disp_drv_init(&disp_drv);  // Basic initialization

  disp_drv.flush_cb = my_disp_flush;  // Set your driver function
  disp_drv.draw_buf = &draw_buf;      // Assign the buffer to the display
  disp_drv.hor_res = LV_HOR_RES_MAX;  // Set the horizontal resolution of the display
  disp_drv.ver_res = LV_VER_RES_MAX;  // Set the vertical resolution of the display

  lv_disp_drv_register(&disp_drv);                   // Finally register the driver
  lv_disp_set_bg_color(NULL, lv_color_hex3(0x000));  // Set default background color to black
}

void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  TouchPoint_t pos = M5.Touch.getPressPoint();
  bool touched = (pos.x == -1) ? false : true;
  if (!touched) {
    data->state = LV_INDEV_STATE_RELEASED;
  } else {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = pos.x;
    data->point.y = pos.y;
  }
}

void init_touch_driver() {
  lv_disp_drv_register(&disp_drv);
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv);  // register
}

static int theme = 1;

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  lv_clock_display(lv_scr_act());
}

static lv_obj_t *clock_display;

static void set_value(void *indic, int32_t v) {
  lv_meter_set_indicator_value(clock_display, (lv_meter_indicator_t *)indic, v);
}

/**
 * A clock display 
 */
void lv_clock_display(lv_obj_t *parent) {
  clock_display = lv_meter_create(parent);
  lv_obj_set_size(clock_display, 220, 220);
  lv_obj_center(clock_display);


  /*Create a scale for the minutes*/
  /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
  lv_meter_scale_t *scale_min = lv_meter_add_scale(clock_display);
  lv_meter_set_scale_ticks(clock_display, scale_min, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_range(clock_display, scale_min, 0, 60, 360, 270);

  /*Create another scale for the hours. It's only visual and contains only major ticks*/
  lv_meter_scale_t *scale_hour = lv_meter_add_scale(clock_display);
  lv_meter_set_scale_ticks(clock_display, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));           /*12 ticks*/
  lv_meter_set_scale_major_ticks(clock_display, scale_hour, 1, 2, 20, lv_palette_main(LV_PALETTE_GREY), 10); /*Every tick is major*/
  lv_meter_set_scale_range(clock_display, scale_hour, 1, 12, 330, 300);                                      /*[1..12] values in an almost full circle*/

  lv_meter_indicator_t *indic_sec = lv_meter_add_needle_line(clock_display, scale_min, 2, lv_palette_main(LV_PALETTE_GREY), -10);
  lv_meter_indicator_t *indic_min = lv_meter_add_needle_line(clock_display, scale_min, 4, lv_palette_main(LV_PALETTE_GREY), -10);
  lv_meter_indicator_t *indic_hour = lv_meter_add_needle_line(clock_display, scale_min, 4, lv_palette_main(LV_PALETTE_GREY), -10);

  /*Create an animation to set the value*/
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, set_value);
  lv_anim_set_values(&a, 0, 60);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_time(&a, 2000); /*2 sec for 1 turn of the minute hand (1 hour)*/
  lv_anim_set_var(&a, indic_min);
  lv_anim_start(&a);

  lv_anim_set_var(&a, indic_hour);
  lv_anim_set_time(&a, 24000); /*24 sec for 1 turn of the hour hand*/
  lv_anim_set_values(&a, 0, 60);
  lv_anim_start(&a);
}

Gesture swipeDown("swipe down", 80, DIR_DOWN, 40);

void loop() {
  M5.update();
  if (swipeDown.wasDetected()) {
    theme = 1 - theme;
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  }
  lv_task_handler();
  lv_tick_inc(500);
}

static void tick_label_event(lv_event_t *e) {
  lv_obj_draw_part_dsc_t *draw_part_dsc = lv_event_get_draw_part_dsc(e);

  /*Be sure it's drawing meter related parts*/
  if (draw_part_dsc->class_p != &lv_meter_class) return;

  /*Be sure it's drawing the ticks*/
  if (draw_part_dsc->type != LV_METER_DRAW_PART_TICK) return;

  /*Be sure it's a major ticks*/
  if (draw_part_dsc->id % 5) return;

  /*The order of numbers on the clock is tricky: 12, 1, 2, 3...*/
  if (draw_part_dsc->id == 0) {
    strncpy(draw_part_dsc->text, "12", 4);
  } else {
    lv_snprintf(draw_part_dsc->text, 4, "%d", draw_part_dsc->id / 5);
  }
}


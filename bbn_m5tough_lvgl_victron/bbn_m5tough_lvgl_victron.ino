#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;    // Descriptor of a display driver
static lv_indev_drv_t indev_drv;  // Descriptor of a touch driver

M5Display *tft;

static void btnPowerOff_event(lv_event_t *event) {
  M5.Axp.PowerOff();
}

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

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();

  lv_obj_t *parent = lv_scr_act();

  static lv_style_t style_shadow;
  lv_style_init(&style_shadow);

  /*Set a background color and a radius*/
  lv_style_set_radius(&style_shadow, 3);
  lv_style_set_shadow_width(&style_shadow, 3);
  lv_style_set_pad_all(&style_shadow, 3);
  lv_style_set_bg_opa(&style_shadow, LV_OPA_COVER);
  lv_style_set_bg_color(&style_shadow, lv_palette_lighten(LV_PALETTE_GREY, 1));

  /*Add border to the bottom+right*/
  lv_style_set_border_color(&style_shadow, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_border_width(&style_shadow, 1);
  lv_style_set_border_opa(&style_shadow, LV_OPA_50);
  lv_style_set_border_side(&style_shadow, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

  lv_obj_t *shore;
  shore = lv_obj_create(parent);
  lv_obj_add_style(shore, &style_shadow, 0);
  lv_obj_set_size(shore, 94, 110);
  lv_obj_align(shore, LV_ALIGN_TOP_LEFT, 5, 5);
  lv_obj_set_style_bg_color(shore, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

  lv_obj_t *shore_label = lv_label_create(shore);
  lv_label_set_text(shore_label, "Shore\n1205 W");
  lv_obj_align(shore_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *inverters;
  inverters = lv_obj_create(parent);
  lv_obj_add_style(inverters, &style_shadow, 0);
  lv_obj_set_size(inverters, 94, 80);
  lv_obj_align(inverters, LV_ALIGN_TOP_MID, 0, 20);
  lv_obj_set_style_bg_color(inverters, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);

  lv_obj_t *inv_label = lv_label_create(inverters);
  lv_label_set_text(inv_label, "Inv");
  lv_obj_align(inv_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *ac;
  ac = lv_obj_create(parent);
  lv_obj_add_style(ac, &style_shadow, 0);
  lv_obj_set_size(ac, 94, 110);
  lv_obj_align(ac, LV_ALIGN_TOP_RIGHT, -5, 5);
  lv_obj_set_style_bg_color(ac, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);

  lv_obj_t *ac_label = lv_label_create(ac);
  lv_label_set_text(ac_label, "AC Load\n340W");
  lv_obj_align(ac_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *dc;
  dc = lv_obj_create(parent);
  lv_obj_add_style(dc, &style_shadow, 0);
  lv_obj_set_size(dc, 94, 110);
  lv_obj_align(dc, LV_ALIGN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_style_bg_color(dc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);

  lv_obj_t *dc_label = lv_label_create(dc);
  lv_label_set_text(dc_label, "Batt\n87%");
  lv_obj_align(dc_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *dc_ld;
  dc_ld = lv_obj_create(parent);
  lv_obj_add_style(dc_ld, &style_shadow, 0);
  lv_obj_set_size(dc_ld, 94, 60);
  lv_obj_align(dc_ld, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_set_style_bg_color(dc_ld, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);

  lv_obj_t *dc_ld_label = lv_label_create(dc_ld);
  lv_label_set_text(dc_ld_label, "DC Load\n150 W");
  lv_obj_align(dc_ld_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *pv;
  pv = lv_obj_create(parent);
  lv_obj_add_style(pv, &style_shadow, 0);
  lv_obj_set_size(pv, 94, 110);
  lv_obj_align(pv, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
  lv_obj_set_style_bg_color(pv, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);

  lv_obj_t *pv_label = lv_label_create(pv);
  lv_label_set_text(pv_label, "PV\n247 W");
  lv_obj_align(pv_label, LV_ALIGN_TOP_LEFT, 0, 0);
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);
}

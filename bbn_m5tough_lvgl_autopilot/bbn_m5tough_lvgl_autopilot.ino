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

static int theme = 0;

void tft_lv_initialization() {
  M5.begin();
  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // Declare a buffer for 1/10 screen siz
  static lv_color_t buf2[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // second buffer is optional

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
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  init_disp_driver();
  init_touch_driver();
  lv_autopilot_buttons(lv_scr_act());
}

Gesture swipeDown("swipe down", 80, DIR_DOWN, 40);

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);
  if (swipeDown.wasDetected()) {
    theme = 1 - theme;
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  }
}

static void event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_DRAW_PART_BEGIN) {
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);

    /*When the button matrix draws the buttons...*/
    if (dsc->class_p == &lv_btnmatrix_class && dsc->type == LV_BTNMATRIX_DRAW_PART_BTN) {
      if (dsc->id == 4) {
        if (lv_btnmatrix_get_selected_btn(obj) == dsc->id) dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_RED, 3);
        else dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);

        dsc->label_dsc->color = lv_color_white();
      }
      dsc->rect_dsc->shadow_width = 6;
      dsc->rect_dsc->shadow_ofs_x = 3;
      dsc->rect_dsc->shadow_ofs_y = 3;
      dsc->rect_dsc->radius = LV_RADIUS_CIRCLE;
    }
  } else if (code == LV_EVENT_VALUE_CHANGED && !swipeDown.wasDetected()) {
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    if (id == 7) {
      theme = 1 - theme;
      lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
    }
  }
}

#define LV_SYMBOL_DOUBLE_LEFT LV_SYMBOL_LEFT " " LV_SYMBOL_LEFT
#define LV_SYMBOL_DOUBLE_RIGHT LV_SYMBOL_RIGHT " " LV_SYMBOL_RIGHT
#define LV_SYMBOL_DEGREES "\xC2\xB0"

static const char *btnm_map[] = {
  LV_SYMBOL_DOUBLE_LEFT, LV_SYMBOL_DOUBLE_RIGHT, "\n",
  LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, "\n",
  "STANDBY", "AUTO", "\n",
  "MODE", LV_SYMBOL_EYE_OPEN, ""
};

void lv_autopilot_buttons(lv_obj_t *parent) {
  lv_obj_t *led = lv_led_create(parent);
  lv_obj_align(led, LV_ALIGN_OUT_LEFT_TOP, 16, 13);
  lv_led_set_color(led, lv_palette_main(LV_PALETTE_GREEN));
  lv_led_on(led);

  int width_l = 128;
  int left_l = 64;
  int top_l = 14;

  lv_obj_t *heading = lv_label_create(parent);
  lv_label_set_text(heading, "HDG:  -120" LV_SYMBOL_DEGREES);
  lv_obj_set_width(heading, width_l);
  lv_obj_set_style_text_align(heading, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(heading, LV_ALIGN_TOP_LEFT, left_l, top_l);
#if LV_FONT_MONTSERRAT_20
  lv_obj_set_style_text_font(heading, &lv_font_montserrat_20, NULL);
#endif

  lv_obj_t *command = lv_label_create(parent);
  lv_label_set_text(command, "CMD:  -120" LV_SYMBOL_DEGREES);
  lv_obj_set_width(command, width_l);
  lv_obj_set_style_text_align(command, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(command, LV_ALIGN_TOP_LEFT, left_l + width_l, top_l);
#if LV_FONT_MONTSERRAT_20
  lv_obj_set_style_text_font(command, &lv_font_montserrat_20, NULL);
#endif

  lv_obj_t *btnm = lv_btnmatrix_create(parent);
  lv_btnmatrix_set_map(btnm, btnm_map);
  lv_btnmatrix_set_btn_width(btnm, 6, 3);
  lv_obj_set_size(btnm, 320, 190);
  lv_obj_add_event_cb(btnm, event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 25);
}

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

static void ta_event_cb(lv_event_t *e);
static lv_obj_t *kb;

const lv_btnmatrix_ctrl_t ctrl_map[] = {
  4, 4, 4,
  4, 4, 4,
  4, 4, 4,
  3, 3, 3, 3
};

const char * btnm_mapplus[10][23] = {
  { "a", "b", "c", "\n",
    "d", "e", "f", "\n",
    "g", "h", "i", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "j", "k", "l", "\n",
    "n", "m", "o", "\n",
    "p", "q", "r", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "s", "t", "u", "\n",
    "v", "w", "x", "\n",
    "y", "z", " ", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "A", "B", "C", "\n",
    "D", "E", "F", "\n",
    "G", "H", "I", "\n",
    LV_SYMBOL_OK, "Del", "Exit" LV_SYMBOL_RIGHT, "" },
  { "J", "K", "L", "\n",
    "N", "M", "O", "\n",
    "P", "Q", "R", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "S", "T", "U", "\n",
    "V", "W", "X", "\n",
    "Y", "Z", " ", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "1", "2", "3", "\n",
    "4", "5", "6", "\n",
    "7", "8", "9", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "0", "+", "-", "\n",
    "/", "*", "=", "\n",
    "!", "?", "#", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "<", ">", "@", "\n",
    "%", "$", "(", "\n",
    ")", "{", "}", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" },
  { "[", "]", ";", "\n",
    "\"", "'", ".", "\n",
    ",", ":", " ", "\n",
    LV_SYMBOL_OK, "Del", "Exit", LV_SYMBOL_RIGHT, "" }
};


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
  lv_example_keyboard(lv_scr_act());
}

static void ta_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
  if (code == LV_EVENT_FOCUSED) {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
}

void lv_example_keyboard(lv_obj_t *parent) {
  // Create a keyboard to use it with all of the text areas
  kb = lv_keyboard_create(parent);
  lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, btnm_mapplus[0], ctrl_map);

  // Create a text area. The keyboard will write here
  lv_obj_t *ta;

  ta = lv_textarea_create(parent);
  lv_obj_align(ta, LV_ALIGN_TOP_RIGHT, -10, 10);
  lv_obj_set_size(ta, 140, 28);
  lv_textarea_set_one_line(ta, true);
  lv_textarea_set_password_mode(ta, false);
  lv_textarea_set_text(ta, "");
  lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
  
  lv_keyboard_set_textarea(kb, ta);
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);
}

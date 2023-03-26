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

static void btnReboot_event(lv_event_t *event) {
  ESP.restart();
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

lv_obj_t *pages[2];
int page = 0;

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();

  lv_obj_t *page1 = lv_obj_create(NULL); // Creates a Screen object
  lv_obj_t *page2 = lv_obj_create(NULL); // Creates another Screen object

  pages[0] = page1;
  pages[1] = page2;

  lv_scr_load(pages[page]);

  lv_obj_t *btn1 = lv_btn_create(page1);
  lv_obj_t *label1 = lv_label_create(btn1);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label1, "Reboot");
  lv_obj_center(label1);
  lv_obj_add_event_cb(btn1, btnReboot_event, LV_EVENT_LONG_PRESSED, NULL);

  lv_obj_t *btn2 = lv_btn_create(page2);
  lv_obj_t *label2 = lv_label_create(btn2);
  lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label2, "Power Off");
  lv_obj_center(label2);
  lv_obj_add_event_cb(btn2, btnPowerOff_event, LV_EVENT_LONG_PRESSED, NULL);
}

Gesture swipe("swipe", 80, DIR_ANY, 40);

void loop() {
  M5.update();
  lv_task_handler();
  if (swipe.wasDetected()) {
    page = 1 - page;
    lv_scr_load(pages[page]);
  }
  lv_tick_inc(1);
}

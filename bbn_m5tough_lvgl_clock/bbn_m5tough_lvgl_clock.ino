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

  static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)) / 2, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
  static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)) / 2, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);

  // Initialize `disp_buf` display buffer with the buffer(s).
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 2);
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

int theme = 1;

static lv_obj_t *clock_display;
lv_meter_indicator_t *indic_sec;
lv_meter_indicator_t *indic_min;
lv_meter_indicator_t *indic_hour;
RTC_TimeTypeDef RTCtime;

static void set_value(void *indic, int32_t v) {
  lv_meter_set_indicator_value(clock_display, (lv_meter_indicator_t *)indic, v);
}

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);

  lv_obj_t *page1 = lv_obj_create(NULL);  // Creates a Screen object
  lv_obj_t *page2 = lv_obj_create(NULL);  // Creates another Screen object

  pages[0] = page1;
  pages[1] = page2;

  lv_clock_display(page1);

  lv_scr_load(pages[page]);

  lv_obj_t *btn1 = lv_btn_create(page2);
  lv_obj_t *label1 = lv_label_create(btn1);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -30);
  lv_label_set_text(label1, "Reboot Device");
  lv_obj_center(label1);
  lv_obj_add_event_cb(btn1, btnReboot_event, LV_EVENT_PRESSED, NULL);
  lv_obj_t *btn2 = lv_btn_create(page2);
  lv_obj_t *label2 = lv_label_create(btn2);
  lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 30);
  lv_label_set_text(label2, "Power Off");
  lv_obj_center(label2);
  lv_obj_add_event_cb(btn2, btnPowerOff_event, LV_EVENT_LONG_PRESSED, NULL);
}

Gesture swipe("swipe", 80, DIR_ANY, 40);

/**
 * A clock display 
 */
void lv_clock_display(lv_obj_t *parent) {
  clock_display = lv_meter_create(parent);
  lv_obj_set_size(clock_display, 220, 220);
  lv_obj_center(clock_display);

  /*Create a scale for the minutes*/
  /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
  lv_meter_scale_t *scale = lv_meter_add_scale(clock_display);
  lv_meter_set_scale_ticks(clock_display, scale, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_range(clock_display, scale, 0, 60, 360, 270);

  /*Create another scale for the hours. It's only visual and contains only major ticks*/
  lv_meter_scale_t *scale_hour = lv_meter_add_scale(clock_display);
  lv_meter_set_scale_ticks(clock_display, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));           /*12 ticks*/
  lv_meter_set_scale_major_ticks(clock_display, scale_hour, 1, 2, 20, lv_palette_main(LV_PALETTE_GREY), 10); /*Every tick is major*/
  lv_meter_set_scale_range(clock_display, scale_hour, 1, 12, 330, 300);                                      /*[1..12] values in an almost full circle*/

  indic_sec = lv_meter_add_needle_line(clock_display, scale, 2, lv_palette_main(LV_PALETTE_GREY), -10);
  indic_min = lv_meter_add_needle_line(clock_display, scale, 4, lv_palette_main(LV_PALETTE_GREEN), -20);
  indic_hour = lv_meter_add_needle_line(clock_display, scale, 7, lv_palette_main(LV_PALETTE_RED), -42);
}

void loop() {
  M5.update();
  lv_task_handler();
  if (swipe.wasDetected()) {
    page = 1 - page;
    lv_scr_load(pages[page]);
  }
  M5.Rtc.GetTime(&RTCtime);
  set_clock_value(indic_hour, ((RTCtime.Hours % 12) * 60 + RTCtime.Minutes) / 12);
  set_value(indic_min, RTCtime.Minutes);
  set_value(indic_sec, RTCtime.Seconds);
  lv_tick_inc(5);
  delay(5);
}

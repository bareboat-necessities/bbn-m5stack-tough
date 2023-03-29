#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

#include "ship_data_model.h"

static ship_data_t shipDataModel;

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient skClient;

using namespace reactesp;
ReactESP app;

#include "signalk_parse.h"
#include "net_signalk_tcp.h"

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;    // Descriptor of a display driver
static lv_indev_drv_t indev_drv;  // Descriptor of a touch driver

M5Display *tft;

void tft_lv_initialization() {
  M5.begin();
  lv_init();

  static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)) / 10, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
  static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc((LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t)) / 10, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);

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
  lv_rudder_display(lv_scr_act());

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  int attemptsCount = 30;
  int status = WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(wifi_ssid);
    status = WiFi.status();
    // wait .5 seconds for connection:
    delay(500);
    M5.Lcd.print(".");
    attemptsCount--;
    if (attemptsCount == 0) return;
  }
  if (status == WL_CONNECTED) {
    M5.Lcd.println("");
    M5.Lcd.println("Connected to " + wifi_ssid);
  }

  static const char *host = "192.168.1.34";  //"lysmarine";
  static int port = 8375;                    // SignalK TCP

  // Connect to the SignalK TCP server
  setup_reconnect(skClient, host, port);
  if (skClient.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);
    signalk_subscribe(skClient);
  } else {
    M5.Lcd.println("Connection failed.");
  }
}

#define LV_SYMBOL_DEGREES "\xC2\xB0"

static lv_obj_t *rudder_display;
static lv_meter_indicator_t *indic_rudder;
static lv_obj_t *rate_of_turn_label;

static void set_value(void *indic, int32_t v) {
  lv_meter_set_indicator_value(rudder_display, (lv_meter_indicator_t *)indic, v);
}

/**
 * A rudder position display
 */
void lv_rudder_display(lv_obj_t *parent) {
  rudder_display = lv_meter_create(parent);
  lv_obj_align(rudder_display, LV_ALIGN_CENTER, 0, -40);
  lv_obj_set_size(rudder_display, 300, 300);
  //v_obj_set_style_arc_width(rudder_display, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(rudder_display, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(rudder_display, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_bg_color(rudder_display, lv_theme_get_color_primary(parent), LV_PART_INDICATOR);

  /*Add a scale first*/
  lv_meter_scale_t *scale = lv_meter_add_scale(rudder_display);
  lv_meter_set_scale_range(rudder_display, scale, -45, 45, 100, 40);
  //lv_linemeter_set_mirror(rudder_display, true);
  lv_meter_set_scale_ticks(rudder_display, scale, 19, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(rudder_display, scale, 3, 3, 20, lv_palette_main(LV_PALETTE_GREY), 15);

  /*Add a needle line indicator*/
  indic_rudder = lv_meter_add_needle_line(rudder_display, scale, 7, lv_theme_get_color_primary(parent), -10);

  rate_of_turn_label = lv_label_create(parent);
  lv_label_set_text(rate_of_turn_label, (String("ROT (" LV_SYMBOL_DEGREES "/min): ")
                                         + String(shipDataModel.navigation.rate_of_turn.deg_sec))
                                          .c_str());
  lv_obj_align(rate_of_turn_label, LV_ALIGN_TOP_LEFT, 5, 5);
}

Gesture swipeDown("swipe down", 80, DIR_DOWN, 40);

void loop() {
  M5.update();
  if (swipeDown.wasDetected()) {
    theme = 1 - theme;
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  }
  lv_task_handler();
  app.tick();

  lv_label_set_text(rate_of_turn_label, (String("ROT (" LV_SYMBOL_DEGREES "/min): ")
                                         + String(shipDataModel.navigation.rate_of_turn.deg_sec))
                                          .c_str());
  lv_tick_inc(1);
}


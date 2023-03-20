#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <Preferences.h>

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;    // Descriptor of a display driver
static lv_indev_drv_t indev_drv;  // Descriptor of a touch driver

M5Display *tft;

// wifi config store.
Preferences preferences;
boolean settingMode;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

static lv_obj_t *list_wifi;

static void ta_event_cb(lv_event_t *e);
static lv_obj_t *kb;

static void setupMode() {
  WiFi.mode(WIFI_STA);  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.disconnect();    // Turn off all wifi connections.
  delay(100);           // 100 ms delay.

  int n = WiFi.scanNetworks();  // return the number of networks found.
  lv_list_wifi(n);
}

boolean checkConnection() {  // Check wifi connection.
  int count = 0;             // count.
  while (count < 30) {       // If you fail to connect to wifi within 30*350ms (10.5s), return false; otherwise return true.
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    delay(350);
    count++;
  }
  return false;
}

static void event_handler_wifi(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED) {
    int n = (int)lv_event_get_user_data(e);
    lv_connect_wifi_win(n);
  }
}

void lv_list_wifi(int num) {
  /*Create a list*/
  list_wifi = lv_list_create(lv_scr_act());
  lv_obj_set_size(list_wifi, 310, 200);
  lv_obj_center(list_wifi);

  /*Add buttons to the list*/
  lv_obj_t *btn;

  lv_list_add_text(list_wifi, "Wi-Fi Networks");

  for (int i = 0; i < num; ++i) {
    btn = lv_list_add_btn(list_wifi, LV_SYMBOL_WIFI, (((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? String("[o] ") : String()) + WiFi.SSID(i)).c_str());
    lv_obj_add_event_cb(btn, event_handler_wifi, LV_EVENT_CLICKED, (void *)i);
    delay(10);
  }
}

static void lv_win_close_event_cb(lv_event_t *e) {
  lv_obj_del((lv_obj_t *)lv_event_get_user_data(e));
}

void lv_connect_wifi_win(int i) {
  lv_obj_t *win = lv_win_create(lv_scr_act(), 36);
  lv_win_add_title(win, "Connect to Wi-Fi");
  lv_obj_t *btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 36);
  lv_obj_add_event_cb(btn, lv_win_close_event_cb, LV_EVENT_CLICKED, win);
  lv_obj_t *cont = lv_win_get_content(win); /*Content can be added here*/
  lv_password_textarea(i, cont);
}

void lv_password_textarea(int i, lv_obj_t *cont) {
  /*Create the password box*/
  lv_obj_t *pwd_ta = lv_textarea_create(cont);
  lv_textarea_set_text(pwd_ta, "");
  //lv_textarea_set_password_mode(pwd_ta, true);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_obj_set_width(pwd_ta, lv_pct(60));
  lv_obj_set_pos(pwd_ta, 16, 5);
  lv_obj_add_event_cb(pwd_ta, ta_password_event_cb, LV_EVENT_ALL, (void *)i);

  /*Create a label and position it above the text box*/
  lv_obj_t *pwd_label = lv_label_create(cont);
  lv_obj_set_width(pwd_label, lv_pct(60));
  lv_label_set_text(pwd_label, "Password: ");
  lv_obj_align_to(pwd_label, pwd_ta, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

  /*Create a keyboard*/
  kb = lv_keyboard_create(cont);
  lv_obj_set_size(kb, LV_HOR_RES, (LV_VER_RES / 2) + 5);

  lv_keyboard_set_textarea(kb, pwd_ta); /*Focus it on one of the text areas to start*/
}

static void ta_password_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  int i = (int)lv_event_get_user_data(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    /*Focus on the clicked text area*/
    if (kb != NULL) lv_keyboard_set_textarea(kb, ta);
  } else if (code == LV_EVENT_READY) {
    preferences.remove("WIFI_SSID");
    preferences.remove("WIFI_PASSWD");
    preferences.putString("WIFI_SSID", WiFi.SSID(i));
    preferences.putString("WIFI_PASSWD", lv_textarea_get_text(ta));
    lv_msgbox(lv_textarea_get_text(ta));
  }
}

static void event_msgbox_cb(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_current_target(e);
  lv_msgbox_close(obj);
  delay(500);
  ESP.restart();
}

static void lv_msgbox(const char *txt) {
  static const char *btns[] = { "Reboot", "" };
  lv_obj_t *mbox = lv_msgbox_create(NULL, "", "Password submitted", btns, true);
  lv_obj_add_event_cb(mbox, event_msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_center(mbox);
}

void tft_lv_initialization() {
  M5.begin();
  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // Declare a buffer for 1/10 screen siz

  // Initialize `disp_buf` display buffer with the buffer(s).
  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10);
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

boolean restoreConfig() {  // Check whether there is wifi configuration information storage, if there is 1 return, if no return 0.
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  return wifi_ssid.length() > 0;
}

static int theme = 0;

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), theme, LV_FONT_DEFAULT);
  preferences.begin("wifi-config");
  delay(10);
  if (restoreConfig()) {      // Check if wifi configuration information has been stored.
    if (checkConnection()) {  // Check wifi connection.
      settingMode = false;    // Turn off setting mode.
      lv_obj_t *labelIP = lv_label_create(lv_scr_act());
      lv_obj_set_pos(labelIP, 10, 10);
      lv_label_set_text(labelIP, (" Wi-Fi:  " + wifi_ssid + "\n"
                                  + " Local IP:  " + WiFi.localIP().toString())
                                   .c_str());
      return;  // Exit setup().
    }
  }
  settingMode = true;  // If there is no stored wifi configuration information, turn on the setting mode.
  setupMode();
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

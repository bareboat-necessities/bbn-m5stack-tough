#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;   // Descriptor of a display driver
static lv_indev_drv_t indev_drv; // Descriptor of a touch driver

M5Display *tft;

static void ta_event_cb(lv_event_t * e);
static lv_obj_t * kb;

static void btnScanWifi_event(lv_event_t * event) {
    WiFi.mode(WIFI_STA);// Set WiFi to station mode and disconnect from an AP if it was previously connected.  
    WiFi.disconnect();  //Turn off all wifi connections.  
    delay(100); //100 ms delay. 

    int n = WiFi.scanNetworks();  //return the number of networks found. 
    lv_list_wifi(n);
}

static lv_obj_t * list_wifi;

static void event_handler_wifi(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
       int n = (int) lv_event_get_user_data(e); 
       M5.Lcd.printf("Clicked: %d %s\n", n, lv_list_get_btn_text(list_wifi, obj));
       lv_connect_wifi_win(n);
    }
}

void lv_list_wifi(int num) {
  /*Create a list*/
  list_wifi = lv_list_create(lv_scr_act());
  lv_obj_set_size(list_wifi, 280, 200);
  lv_obj_center(list_wifi);

  /*Add buttons to the list*/
  lv_obj_t * btn;

  lv_list_add_text(list_wifi, "Wi-Fi Networks");

  for (int i = 0; i < num; ++i) {
    //Print SSID and RSSI for each network found. 
    //M5.Lcd.printf("%d:",i + 1);
    //M5.Lcd.print(WiFi.SSID(i));
    //M5.Lcd.printf("(%d)",WiFi.RSSI(i));
    //M5.Lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
    btn = lv_list_add_btn(list_wifi, LV_SYMBOL_WIFI, WiFi.SSID(i).c_str());
    lv_obj_add_event_cb(btn, event_handler_wifi, LV_EVENT_CLICKED, (void*) i);
    delay(10);
  }
}

static void event_handler_connect_wifi_win(lv_event_t * e) {
  lv_obj_t * obj = lv_event_get_target(e);
  LV_LOG_USER("Button %d clicked", (int)lv_obj_get_index(obj));
}

static void lv_win_close_event_cb(lv_event_t * e) {
  lv_obj_del((lv_obj_t *) lv_event_get_user_data(e));
}

void lv_connect_wifi_win(int i) {
  lv_obj_t * win = lv_win_create(lv_scr_act(), 40);
  lv_win_add_title(win, "Connect to Wi-Fi");
  lv_obj_t * btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 40);
  lv_obj_add_event_cb(btn, lv_win_close_event_cb, LV_EVENT_CLICKED, win);
  lv_obj_t * cont = lv_win_get_content(win);  /*Content can be added here*/
  lv_password_textarea(i, cont);
}

void lv_password_textarea(int i, lv_obj_t * cont) {
    /*Create the password box*/
    lv_obj_t * pwd_ta = lv_textarea_create(cont);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_obj_set_width(pwd_ta, lv_pct(80));
    lv_obj_set_pos(pwd_ta, 4, 6);
    lv_obj_add_event_cb(pwd_ta, ta_password_event_cb, LV_EVENT_ALL, NULL);

    /*Create a label and position it above the text box*/
    lv_obj_t * pwd_label = lv_label_create(cont);
    lv_obj_set_width(pwd_label, lv_pct(80));
    lv_label_set_text(pwd_label, "Password:" /*("Password to " + WiFi.SSID(i)).c_str()*/);
    lv_obj_align_to(pwd_label, pwd_ta, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

    /*Create a keyboard*/
    kb = lv_keyboard_create(cont);
    lv_obj_set_size(kb, LV_HOR_RES, LV_VER_RES / 2);

    lv_keyboard_set_textarea(kb, pwd_ta); /*Focus it on one of the text areas to start*/
}

static void ta_password_event_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    /*Focus on the clicked text area*/
    if (kb != NULL) lv_keyboard_set_textarea(kb, ta);
  }
  else if (code == LV_EVENT_READY) {
    LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
  }
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

void my_touchpad_read(lv_indev_drv_t * drv, lv_indev_data_t * data) {
  TouchPoint_t pos = M5.Touch.getPressPoint();
  bool touched = ( pos.x == -1 ) ? false : true;  
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
  lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);  // register
}

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
  lv_obj_t * label = lv_label_create(btn1);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label, "Scan Wi-Fi");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn1, btnScanWifi_event, LV_EVENT_CLICKED, NULL);
}

void loop() {
  M5.update();
  lv_task_handler();
  lv_tick_inc(1);
}


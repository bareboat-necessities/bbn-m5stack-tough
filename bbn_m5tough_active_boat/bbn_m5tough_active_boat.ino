#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>
#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#undef min(a, b)
#include <ReactESP.h>

#include "ui_init.h"
#include "ui_gestures.h"
#include "ui_theme.h"
#include "ui_screens.h"
#include "m5_rtc.h"
#include "ui_clock.h"
#include "ui_about.h"

// config store.
Preferences preferences;

#include "ui_keyboard.h"
#include "ui_settings_wifi.h"
#include "ui_reboot.h"

using namespace reactesp;
ReactESP app;

#include "ship_data_model.h"
static ship_data_t shipDataModel;

#include "signalk_parse.h"
#include "net_signalk_tcp.h"

#include "ui_wind.h"
#include "ui_heel.h"
#include "ui_autopilot.h"
#include "ui_rudder.h"
#include "ui_power_victron.h"

lv_updatable_screen_t* screens[] = {
  &windScreen,
  &aboutScreen,
  &rebootScreen,
  &clockScreen,
  &victronScreen,
  &autopilotScreen,
  &heelScreen,
  &rudderScreen,
};

int page = 0;
int pages_count = sizeof(screens) / sizeof(screens[0]);

void next_page() {
  page++;
  if (page >= pages_count) page = 0;
  lv_scr_load(screens[page]->screen);
}

void prev_page() {
  page--;
  if (page < 0) page = pages_count - 1;
  lv_scr_load(screens[page]->screen);
}

void handle_swipe() {
  if (swipe_vert_detected()) {
    toggle_ui_theme();
  } else if (swipe_right_detected()) {
    next_page();
  } else if (swipe_left_detected()) {
    prev_page();
  }
}

WiFiClient skClient;

void setup() {
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  init_theme();

  settingUpWiFi([&page, &screens]() {
    init_windScreen();
    init_aboutScreen();
    init_rebootScreen();
    init_clockScreen();
    init_victronScreen();
    init_autopilotScreen();
    init_heelScreen();
    init_rudderScreen();
    lv_scr_load(screens[page]->screen);

    // TODO: autodisconer and read from preferences
    static const char* host = "192.168.1.34";  //"lysmarine";
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
  });
}

void loop() {
  M5.update();
  lv_task_handler();
  app.tick();
  lv_tick_inc(1);

  if (!settingMode) {
    handle_swipe();
    update_screen(*screens[page]);
  }
}

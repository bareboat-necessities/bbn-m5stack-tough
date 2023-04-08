#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

#include <M5Tough.h>  // https://github.com/m5stack/M5Tough
#include <Arduino.h>
#include <time.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <mdns.h>
#include <MQTT.h>  // https://github.com/256dpi/arduino-mqtt
#include <Preferences.h>
#include <ArduinoJson.h>
#undef min(a, b)
#include <ReactESP.h>  // https://github.com/mairas/ReactESP

#include "keepalive.h"
#include "m5_rtc.h"
#include "m5_sound.h"
#include "net_ntp_time.h"
#include "net_mdns.h"
#include "net_mqtt.h"
#include "ui_init.h"
#include "ui_gestures.h"
#include "ui_theme.h"
#include "ui_screens.h"
#include "ui_clock.h"
#include "ui_player_control.h"
#include "ui_about.h"
#include "ui_brightness.h"
#include "ui_mem_cpu_net_stat.h"

// config store.
Preferences preferences;

#include "ui_keyboard.h"
#include "ui_settings_wifi.h"
#include "ui_reboot.h"

using namespace reactesp;
ReactESP app;

#include "ship_data_model.h"
#include "ship_data_util.h"

static ship_data_t shipDataModel;

#include "WMM_Tinier.h"

WMM_Tinier myDeclination;

#include "derived_data.h"
#include "TinyGPSPlus.h"
#include "nmea0183_parse.h"
#include "net_nmea0183_tcp.h"
#include "signalk_parse.h"
#include "net_signalk_tcp.h"
#include "pypilot_parse.h"
#include "net_pypilot.h"

WiFiClient nmea0183Client;
WiFiClient skClient;
WiFiClient pypClient;
MQTTClient mqttClient;

#include "ui_compass.h"
#include "ui_wind.h"
#include "ui_engine.h"
#include "ui_gps.h"
#include "ui_speed.h"
#include "ui_depth.h"
#include "ui_heel.h"
#include "ui_autopilot.h"
#include "ui_rudder.h"
#include "ui_power_victron.h"

lv_updatable_screen_t* screens[] = {
  &windScreen,
  &aboutScreen,
  &rebootScreen,
  &clockScreen,
  &playerScreen,
  &victronScreen,
  &autopilotScreen,
  &heelScreen,
  &compassScreen,
  &rudderScreen,
  &engineScreen,
  &gpsScreen,
  &speedScreen,
  &depthScreen,
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

void setup() {
  myDeclination.begin();
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  init_theme();

  settingUpWiFi([&page, &screens]() {
    init_dateTime();
    init_windScreen();
    init_engineScreen();
    init_gpsScreen();
    init_speedScreen();
    init_depthScreen();
    init_aboutScreen();
    init_rebootScreen();
    init_clockScreen();
    init_playerScreen();
    init_victronScreen();
    init_autopilotScreen();
    init_heelScreen();
    init_compassScreen();
    init_rudderScreen();
    lv_scr_load(screens[page]->screen);

    // TODO: auto-discover and read from preferences
    static const char* host = "192.168.1.34";  //"lysmarine";
    static int port = 8375;                    // SignalK TCP
    signalk_begin(skClient, host, port);       // Connect to the SignalK TCP server

    // TODO: auto-discover and read from preferences
    static const char* pyp_host = "192.168.1.34";  //"lysmarine";
    static int pyp_port = 23322;                   // Pypilot TCP
    pypilot_begin(pypClient, pyp_host, pyp_port);  // Connect to the PyPilot TCP server

    // TODO: auto-discover and read from preferences
    static const char* nmea0183_host = "192.168.1.34";                 //"lysmarine";
    static int nmea0183_port = 10110;                                  // NMEA0183 TCP
    nmea0183_tcp_begin(nmea0183Client, nmea0183_host, nmea0183_port);  // Connect to the NMEA 0183 TCP server
  });

  SpeakerInit();
  DingDong();
}

unsigned long last_ui_upd = 0;

void loop() {
  M5.update();
  lv_task_handler();
  app.tick();
  lv_tick_inc(1);
  //mqttClient.loop();

  if (!settingMode) {
    handle_swipe();
    if (millis() - last_ui_upd > 250) {  // throttle expensive UI updates, and calculations
      derive_data();
      update_screen(*screens[page]);
      last_ui_upd = millis();
    }
  }
}

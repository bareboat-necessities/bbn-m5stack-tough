#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

//#define ENABLE_SCREEN_SERVER // This server sends screenshots via serial to processing.org sketch running on PC (not for production)

#undef ENABLE_MPD
//
//
//  Changes to lv_conf.h (LVGL 8.x) needed:
//
//   /* clang-format off */
//   #if 1 /*Set it to "1" to enable content*/
// 
//   #define LV_COLOR_DEPTH 16
//
//   #define LV_COLOR_SCREEN_TRANSP 1
//
//   #define LV_FONT_MONTSERRAT_20 1
//
//   /*Use a custom tick source that tells the elapsed time in milliseconds.
//   /*It removes the need to manually update the tick with `lv_tick_inc()`)*/
//   #define LV_TICK_CUSTOM 1
//   #if LV_TICK_CUSTOM
//       #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"         /*Header for the system time function*/
//       #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())    /*Expression evaluating to current system time in ms*/
//
//


#include <M5Tough.h>  // https://github.com/m5stack/M5Tough
#include <lvgl.h>
#include <Arduino.h>
#include <time.h>
#include <Wire.h>
#include <SPI.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <mdns.h>
#include <MQTT.h>  // https://github.com/256dpi/arduino-mqtt
#include <HTTPClient.h>
#include <Preferences.h>

// config store.
Preferences preferences;

#include <ArduinoJson.h>
#undef min(a, b)
#include <ReactESP.h>  // https://github.com/mairas/ReactESP

typedef struct _NetClient {
  WiFiClient c = WiFiClient();
  unsigned long lastActivity = 0U;
} NetClient;

#include "keepalive.h"
#include "hw_brightness.h"
#include "hw_rtc.h"
#include "hw_sound.h"
#include "net_ntp_time.h"
#include "net_mdns.h"
#include "net_http.h"
#include "ui_init.h"
#include "ui_gestures.h"
#include "ui_theme.h"
#include "ui_screens.h"
#include "ui_settings.h"
#include "ui_clock.h"

#ifdef ENABLE_SCREEN_SERVER // This server sends screenshots via serial to processing.org sketch running on PC
// See https://github.com/Bodmer/TFT_eSPI/blob/master/examples/Generic/TFT_Screen_Capture/processing_sketch.ino
// Set the baud rate in it to default M5Stack as int serial_baud_rate = 115200; 
#include "screenServer.h"
#endif

#ifdef ENABLE_MPD // TODO:
#include "ui_player_control.h"
#endif

#include "ui_about.h"

#include "ui_keyboard.h"
#include "ui_settings_wifi.h"
#include "ui_reboot.h"

using namespace reactesp;
ReactESP app;

#include "ship_data_model.h"

static ship_data_t shipDataModel;

#include "ship_data_util.h"

#include "WMM_Tinier.h"

WMM_Tinier myDeclination;

#include "sunriset.h"
#include "derived_data.h"
#include "TinyGPSPlus.h"
#include "nmea0183_parse.h"
#include "net_nmea0183_tcp.h"
#include "net_mqtt.h"
#include "signalk_parse.h"
#include "net_signalk_http.h"
#include "net_signalk_tcp.h"
#include "pypilot_parse.h"
#include "net_pypilot.h"

NetClient nmea0183Client;
NetClient skClient;
NetClient pypClient;
WiFiClient mqttNetClient;
MQTTClient mqttClient = MQTTClient(4096); // Data loss if buffer is not enough
static bool victron_mqtt_began = false;

#include "ui_ip_add_editor.h"
#include "ui_mem_cpu_net_stat.h"
#include "ui_compass.h"
#include "ui_wind.h"
#include "ui_engine.h"
#include "ui_gps.h"
#include "ui_speed.h"
#include "ui_depth.h"
#include "ui_heel.h"
#include "ui_trip_data.h"
#include "ui_autopilot.h"
#include "ui_rudder.h"
#include "ui_tanks.h"
#include "ui_weather.h"
#include "ui_power_victron.h"
#include "ui_vessel_info.h"

lv_updatable_screen_t* screens[] = {
  
  &windScreen,
  &heelScreen,
  &rudderScreen,
  &engineScreen,
  &autopilotScreen,
#ifdef ENABLE_MPD // TODO:
  &playerScreen,
#endif
  &victronScreen,
  &tanksScreen,
  &vesselScreen,
  &rebootScreen,
  &aboutScreen,
  &devStatusScreen,
  &clockScreen,
  &compassScreen,
  &tripDataScreen,
  &weatherScreen,
  &gpsScreen,
  &speedScreen,
  &depthScreen,
};

int page = 0;
int pages_count = sizeof(screens) / sizeof(screens[0]);

void next_page() {
  lv_obj_clean(screens[page]->screen);
  page++;
  if (page >= pages_count) page = 0;
  init_screen(*screens[page]);
  lv_scr_load(screens[page]->screen);
}

void prev_page() {
  lv_obj_clean(screens[page]->screen);
  page--;
  if (page < 0) page = pages_count - 1;
  init_screen(*screens[page]);
  lv_scr_load(screens[page]->screen);
}

bool handle_swipe() {
  if (swipe_vert_detected()) {
    toggle_ui_theme(); 
    return true;
  } else if (swipe_right_detected()) {
    next_page();
    return true;
  } else if (swipe_left_detected()) {
    prev_page();
    return true;
  }
  return false;
}

void setup() {
  myDeclination.begin();
  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();
  init_theme();
  rtc_begin();

  settingUpWiFi([]() {
    init_dateTime();
    
    init_windScreen();
    init_weatherScreen();
    init_engineScreen();
    init_gpsScreen();
    init_speedScreen();
    init_depthScreen();
    init_clockScreen();
#ifdef ENABLE_MPD // TODO:
    init_playerScreen();
#endif
    init_victronScreen();
    init_tanksScreen();
    init_autopilotScreen();
    init_heelScreen();
    init_tripDataScreen();
    init_compassScreen();
    init_rudderScreen();
    init_aboutScreen();
    init_rebootScreen();
    init_devStatusScreen();
    init_vesselScreen();
  
    init_screen(*screens[page]);
    lv_scr_load(screens[page]->screen);

    discover_n_config();  // Discover services via mDNS

    static String signalk_tcp_host = preferences.getString(SK_TCP_HOST_PREF);
    static int signalk_tcp_port = preferences.getInt(SK_TCP_PORT_PREF);
    if (signalk_tcp_host.length() > 0 && signalk_tcp_port > 0) {
      signalk_begin(skClient, signalk_tcp_host.c_str(), signalk_tcp_port);  // Connect to the SignalK TCP server
    }

    static String pypilot_tcp_host = preferences.getString(PYP_TCP_HOST_PREF);
    static int pypilot_tcp_port = preferences.getInt(PYP_TCP_PORT_PREF);
    if (pypilot_tcp_host.length() > 0 && pypilot_tcp_port > 0) {
      pypilot_begin(pypClient, pypilot_tcp_host.c_str(), pypilot_tcp_port);  // Connect to the PyPilot TCP server
    }

    static String nmea0183_tcp_host = preferences.getString(NMEA0183_TCP_HOST_PREF);
    static int nmea0183_tcp_port = preferences.getInt(NMEA0183_TCP_PORT_PREF);
    if (nmea0183_tcp_host.length() > 0 && nmea0183_tcp_port > 0) {
      nmea0183_tcp_begin(nmea0183Client, nmea0183_tcp_host.c_str(), nmea0183_tcp_port);  // Connect to the NMEA 0183 TCP server
    }

    static String victron_mqtt_host = preferences.getString(VENUS_MQTT_HOST_PREF);
    static int victron_mqtt_port = preferences.getInt(VENUS_MQTT_PORT_PREF);
    if (victron_mqtt_host.length() > 0 && victron_mqtt_port > 0) {
      victron_mqtt_client_begin(mqttClient, mqttNetClient, victron_mqtt_host.c_str(), victron_mqtt_port);
      victron_mqtt_began = true;
    }

    app.onDelay(400, []() {
      getVesselInfo();
    });
  });

  SpeakerInit();
  DingDong();
}

unsigned long last_ui_upd = 0;

void loop() {
  M5.update();
  lv_task_handler();
  app.tick();
#if !(LV_TICK_CUSTOM)
  lv_tick_inc(1);
#endif

  if (!settingMode) {
    if (victron_mqtt_began) {
      victron_mqtt_client_loop(mqttClient);
    }
    bool detected = handle_swipe();
    if (detected || (millis() - last_ui_upd > 300)) {  // throttle expensive UI updates, and calculations
      derive_data();
      update_screen(*screens[page]);
      last_ui_upd = millis();
    }
#ifdef ENABLE_SCREEN_SERVER 
    // (not for production)
    if (detected) {
      screenServer0();
    }
#endif
  }
}

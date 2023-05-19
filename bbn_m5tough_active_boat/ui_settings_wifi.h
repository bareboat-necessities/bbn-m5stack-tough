#ifndef UI_SETTINGS_WIFI_H
#define UI_SETTINGS_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

  boolean settingMode;

  static lv_obj_t *list_wifi;

  boolean restoreConfig() {  // Check whether there is wifi configuration information storage, if there is return 1, if no return 0.
    wifi_ssid = preferences.getString("WIFI_SSID");
    wifi_password = preferences.getString("WIFI_PASSWD");
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    return wifi_ssid.length() > 0;
  }

  static void btnResetWiFiSettings_event(lv_event_t *event) {
    preferences.remove("WIFI_SSID");
    preferences.remove("WIFI_PASSWD");
    ESP_restart();
  }

  void wifi_connected(void (*on_connected)()) {
    (*on_connected)();
    settingMode = false;  // Turn off setting mode.
  }

  static inline int8_t dBm_to_percents(int8_t dBm) {
    int quality;
    // dBm to Quality:
    if (dBm <= -100)
      quality = 0;
    else if (dBm >= -50)
      quality = 100;
    else
      quality = 2 * (dBm + 100);
    return quality;
  }

  static void lv_win_close_event_cb(lv_event_t *e) {
    lv_obj_del((lv_obj_t *)lv_event_get_user_data(e));
  }

  static void event_msgbox_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target(e);
    lv_msgbox_close(obj);
    delay(100);
    ESP.restart();
  }

  static void lv_msgbox(const char *txt) {
    static const char *btns[] PROGMEM = { "Reboot", "" };
    lv_obj_t *mbox = lv_msgbox_create(NULL, "", "Password submitted", btns, true);
    lv_obj_add_event_cb(mbox, event_msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox);
  }

  static void ta_password_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    int i = (int)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
      /*Focus on the clicked text area*/
      if (kb != NULL) lv_keyboard_set_textarea(kb, ta);
#ifdef ENABLE_SCREEN_SERVER
      // (not for production)
      screenServer0();
#endif
    } else if (code == LV_EVENT_READY) {
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      preferences.putString("WIFI_SSID", WiFi.SSID(i));
      preferences.putString("WIFI_PASSWD", lv_textarea_get_text(ta));
      lv_msgbox(lv_textarea_get_text(ta));
    }
  }

  void lv_password_textarea(int i, lv_obj_t *cont) {
    /*Create the password box*/
    lv_obj_set_style_pad_all(cont, 2, LV_PART_MAIN);
    lv_obj_t *pwd_ta = lv_textarea_create(cont);
    lv_textarea_set_text(pwd_ta, "");
    //lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_obj_set_width(pwd_ta, 230);
    lv_obj_align(pwd_ta, LV_ALIGN_TOP_LEFT, 2, 2);
    lv_obj_add_event_cb(pwd_ta, ta_password_event_cb, LV_EVENT_ALL, (void *)i);

    /*Create a keyboard*/
    kb = lv_keyboard2(cont);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_size(kb, LV_HOR_RES - 4, (LV_VER_RES / 2) + 48);

    lv_keyboard_set_textarea(kb, pwd_ta); /*Focus it on one of the text areas to start*/
    lv_obj_add_state(pwd_ta, LV_STATE_FOCUSED);
  }

  void lv_connect_wifi_win(int i) {
    lv_obj_t *win = lv_win_create(lv_scr_act(), 28);
    lv_win_add_title(win, (String(" Wi-Fi Password: ") += String(WiFi.SSID(i)).substring(0, 9) += "...").c_str());
    lv_obj_t *btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 28);
    lv_obj_add_event_cb(btn, lv_win_close_event_cb, LV_EVENT_PRESSED, win);
    lv_obj_t *cont = lv_win_get_content(win); /*Content can be added here*/
    lv_password_textarea(i, cont);
  }

  static void event_handler_wifi(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_LONG_PRESSED) {
      int n = (int)lv_event_get_user_data(e);
#ifdef ENABLE_SCREEN_SERVER
      // (not for production)
      screenServer0();
#endif
      lv_connect_wifi_win(n);
    }
  }

  void lv_list_wifi(lv_obj_t *parent, int num) {
    /*Create a list*/
    list_wifi = lv_list_create(parent);
    lv_obj_set_size(list_wifi, 310, 200);
    lv_obj_center(list_wifi);

    /*Add buttons to the list*/
    lv_obj_t *btn;

    lv_list_add_text(list_wifi, "Wi-Fi Networks");

    for (int i = 0; i < num; ++i) {
      btn = lv_list_add_btn(list_wifi, (String(LV_SYMBOL_WIFI) += String("  ") += String(dBm_to_percents(WiFi.RSSI(i))) += "%").c_str(),
                            (((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? String(LV_SYMBOL_EYE_OPEN " ") : String("")) += WiFi.SSID(i)).c_str());

      lv_obj_add_event_cb(btn, event_handler_wifi, LV_EVENT_LONG_PRESSED, (void *)i);
    }
  }

  static void setupMode(void (*on_connected)()) {
    WiFi.mode(WIFI_STA);  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.disconnect();    // Turn off all wifi connections.
    delay(100);           // 100 ms delay.

    int n = WiFi.scanNetworks();  // return the number of networks found.

    lv_list_wifi(lv_scr_act(), n);
    restoreConfig();  // to allow reconnect while in setup mode

    WiFi.onEvent([&](WiFiEvent_t event, WiFiEventInfo_t info) {
      if (list_wifi != NULL) {
        lv_obj_del(list_wifi);
        list_wifi = NULL;
      }
      delay(2000);
      //wifi_connected(on_connected);
      ESP.restart();  // can just restart for clean attempt
    },
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
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

  void settingUpWiFi(void (*on_connected)()) {
    preferences.begin("wifi-config", false);
    if (restoreConfig()) {      // Check if wifi configuration information has been stored.
      if (checkConnection()) {  // Check wifi connection.
        wifi_connected(on_connected);
        return;
      }
    }
    settingMode = true;  // If there is no stored wifi configuration information, turn on the setting mode.
    setupMode(on_connected);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

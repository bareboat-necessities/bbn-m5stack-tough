#ifndef MEM_CPU_NET_STAT_H
#define MEM_CPU_NET_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t devStatusScreen;

  static lv_obj_t *pyp_status_label;
  static lv_obj_t *nmea0183_status_label;
  static lv_obj_t *signalk_status_label;
  static lv_obj_t *venus_mqtt_status_label;
  static lv_obj_t *mpd_status_label;
  static lv_obj_t *data_connections_list;

  static void event_handler_data_conn(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    const char *host_type = (const char *)lv_event_get_user_data(e);
    lv_obj_add_flag(data_connections_list, LV_OBJ_FLAG_HIDDEN);
    if (code == LV_EVENT_CLICKED) {
      if (strcmp(host_type, VENUS_MQTT_HOST_PREF) == 0) {
        String venus_mqtt_host = preferences.getString(VENUS_MQTT_HOST_PREF);
        int venus_mqtt_port = preferences.getInt(VENUS_MQTT_PORT_PREF);
        if (venus_mqtt_host.length() == 0 || venus_mqtt_host == "0.0.0.0") {
          venus_mqtt_host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(venus_mqtt_host.c_str(), (int32_t) venus_mqtt_port, "VE Venus MQTT");
      } else {
        // TODO:
      }
    }
  }

  void lv_data_connections(lv_obj_t *parent) {
    data_connections_list = lv_list_create(parent);
    lv_obj_set_size(data_connections_list, 220, 180);
    lv_obj_align(data_connections_list, LV_ALIGN_CENTER, 0, 19);

    lv_obj_t *btn;
    btn = lv_list_add_btn(data_connections_list, NULL, "SignalK TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)SK_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "PyPilot TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)PYP_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "NMEA 0183 TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)NMEA0183_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "VE Venus MQTT");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)VENUS_MQTT_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "MPD Player");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)MPD_TCP_HOST_PREF);

    lv_ip_addr_editor(parent);
    lv_ip_addr_editor_hide();
  }
/*
  void printDeviceStats() {
    M5.Lcd.printf("FreeHeap: %d bytes\n", ESP.getFreeHeap());
    M5.Lcd.printf("MinFreeHeap: %d bytes\n", ESP.getMinFreeHeap());
    M5.Lcd.printf("FreePsram: %d bytes\n", ESP.getFreePsram());
    M5.Lcd.printf("MinFreePsram: %d bytes\n", ESP.getMinFreePsram());
    M5.Lcd.printf("FreeSketchSpace: %d bytes\n", ESP.getFreeSketchSpace());
    M5.Lcd.printf("HeapSize: %d bytes\n", ESP.getHeapSize());

    M5.Lcd.printf("FlashChipSize: %d bytes\n", ESP.getFlashChipSize());
    M5.Lcd.printf("MaxAllocHeap: %d bytes\n", ESP.getMaxAllocHeap());
    M5.Lcd.printf("MaxAllocPsram: %d bytes\n", ESP.getMaxAllocPsram());
    M5.Lcd.printf("ChipCores: %d\n", ESP.getChipCores());
  }
*/
  static void erase_net_conf_evt_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_LONG_PRESSED) {
      erase_mdns_lookups();
      ESP.restart();
    }
  }

  static void edit_net_conf_evt_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
      lv_obj_clear_flag(data_connections_list, LV_OBJ_FLAG_HIDDEN);
    }
  }

  /**
   * A dev status display 
   */
  void lv_dev_status_display(lv_obj_t *parent) {
    pyp_status_label = lv_label_create(parent);
    lv_obj_align(pyp_status_label, LV_ALIGN_TOP_LEFT, 20, 30);
    lv_label_set_recolor(pyp_status_label, true);
    lv_label_set_text(pyp_status_label,
                      (String("#ff0000 " LV_SYMBOL_OK "  #") + String("PyPilot:              ") + preferences.getString(PYP_TCP_HOST_PREF) + String(":") + String(preferences.getInt(PYP_TCP_PORT_PREF))).c_str());

    signalk_status_label = lv_label_create(parent);
    lv_obj_align(signalk_status_label, LV_ALIGN_TOP_LEFT, 20, 60);
    lv_label_set_recolor(signalk_status_label, true);
    lv_label_set_text(signalk_status_label,
                      (String("#ff0000 " LV_SYMBOL_OK "  #") + String("SignalK:             ") + preferences.getString(SK_TCP_HOST_PREF) + String(":") + String(preferences.getInt(SK_TCP_PORT_PREF))).c_str());

    nmea0183_status_label = lv_label_create(parent);
    lv_obj_align(nmea0183_status_label, LV_ALIGN_TOP_LEFT, 20, 90);
    lv_label_set_recolor(nmea0183_status_label, true);
    lv_label_set_text(nmea0183_status_label,
                      (String("#ff0000 " LV_SYMBOL_OK "  #") + String("NMEA 0183:       ") + preferences.getString(NMEA0183_TCP_HOST_PREF) + String(":") + String(preferences.getInt(NMEA0183_TCP_PORT_PREF))).c_str());

    venus_mqtt_status_label = lv_label_create(parent);
    lv_obj_align(venus_mqtt_status_label, LV_ALIGN_TOP_LEFT, 20, 120);
    lv_label_set_recolor(venus_mqtt_status_label, true);
    lv_label_set_text(venus_mqtt_status_label,
                      (String("#ff0000 " LV_SYMBOL_OK "  #") + String("Venus MQTT:    ") + preferences.getString(VENUS_MQTT_HOST_PREF) + String(":") + String(preferences.getInt(VENUS_MQTT_PORT_PREF))).c_str());

    mpd_status_label = lv_label_create(parent);
    lv_obj_align(mpd_status_label, LV_ALIGN_TOP_LEFT, 20, 150);
    lv_label_set_recolor(mpd_status_label, true);
    lv_label_set_text(mpd_status_label,
                      (String("#ff0000 " LV_SYMBOL_OK "  #") + String("MPD:                  ") + preferences.getString(MPD_TCP_HOST_PREF) + String(":") + String(preferences.getInt(MPD_TCP_PORT_PREF))).c_str());

    lv_obj_t *btn_erase = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_erase, erase_net_conf_evt_handler, LV_EVENT_ALL, parent);
    lv_obj_align(btn_erase, LV_ALIGN_CENTER, 40, 90);
    lv_obj_t *btn_erase_l = lv_label_create(btn_erase);
    lv_label_set_text(btn_erase_l, LV_SYMBOL_TRASH);

    lv_obj_t *btn_edit = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_edit, edit_net_conf_evt_handler, LV_EVENT_ALL, parent);
    lv_obj_align(btn_edit, LV_ALIGN_CENTER, -40, 90);
    lv_obj_t *btn_edit_l = lv_label_create(btn_edit);
    lv_label_set_text(btn_edit_l, LV_SYMBOL_SETTINGS);

    lv_data_connections(parent);
    lv_obj_add_flag(data_connections_list, LV_OBJ_FLAG_HIDDEN);
  }

  static void dev_status_update_cb() {
    lv_label_set_text(pyp_status_label,
                      ((pypClient.connected() ? String("#00ff00 " LV_SYMBOL_OK "  #")
                                              : String("#ff0000 " LV_SYMBOL_OK "  #"))
                       + String("PyPilot:              ") + preferences.getString(PYP_TCP_HOST_PREF)
                       + String(":") + String(preferences.getInt(PYP_TCP_PORT_PREF)))
                        .c_str());
    lv_label_set_text(signalk_status_label,
                      ((skClient.connected() ? String("#00ff00 " LV_SYMBOL_OK "  #")
                                             : String("#ff0000 " LV_SYMBOL_OK "  #"))
                       + String("SignalK:             ") + preferences.getString(SK_TCP_HOST_PREF)
                       + String(":") + String(preferences.getInt(SK_TCP_PORT_PREF)))
                        .c_str());
    lv_label_set_text(nmea0183_status_label,
                      ((nmea0183Client.connected() ? String("#00ff00 " LV_SYMBOL_OK "  #")
                                                   : String("#ff0000 " LV_SYMBOL_OK "  #"))
                       + String("NMEA 0183:       ") + preferences.getString(NMEA0183_TCP_HOST_PREF)
                       + String(":") + String(preferences.getInt(NMEA0183_TCP_PORT_PREF)))
                        .c_str());
  }

  void init_devStatusScreen() {
    devStatusScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_dev_status_display(devStatusScreen.screen);
    devStatusScreen.update_cb = dev_status_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

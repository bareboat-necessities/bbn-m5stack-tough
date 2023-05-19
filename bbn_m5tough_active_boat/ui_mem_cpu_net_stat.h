#ifndef MEM_CPU_NET_STAT_H
#define MEM_CPU_NET_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

  static const char *PROGMEM UI_BLANK_STR = " ";

  lv_updatable_screen_t devStatusScreen;

  static lv_obj_t *pyp_status_label;
  static lv_obj_t *nmea0183_status_label;
  static lv_obj_t *signalk_status_label;
  static lv_obj_t *venus_mqtt_status_label;
#ifdef ENABLE_MPD // TODO:
  static lv_obj_t *mpd_status_label;
#endif
  static lv_obj_t *data_connections_list;

  static void event_handler_data_conn(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    const char *host_type = (const char *)lv_event_get_user_data(e);
    lv_obj_add_flag(data_connections_list, LV_OBJ_FLAG_HIDDEN);
    if (code == LV_EVENT_CLICKED) {
      if (strcmp(host_type, VENUS_MQTT_HOST_PREF) == 0) {
        String host = preferences.getString(VENUS_MQTT_HOST_PREF);
        int port = preferences.getInt(VENUS_MQTT_PORT_PREF);
        if (host.length() == 0 || host == "0.0.0.0") {
          host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(host.c_str(), (int32_t)port, "VE Venus MQTT", VENUS_MQTT_HOST_PREF);
      } else if (strcmp(host_type, PYP_TCP_HOST_PREF) == 0) {
        String host = preferences.getString(PYP_TCP_HOST_PREF);
        int port = preferences.getInt(PYP_TCP_PORT_PREF);
        if (host.length() == 0 || host == "0.0.0.0") {
          host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(host.c_str(), (int32_t)port, "PyPilot TCP", PYP_TCP_HOST_PREF);
      } else if (strcmp(host_type, SK_TCP_HOST_PREF) == 0) {
        String host = preferences.getString(SK_TCP_HOST_PREF);
        int port = preferences.getInt(SK_TCP_PORT_PREF);
        if (host.length() == 0 || host == "0.0.0.0") {
          host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(host.c_str(), (int32_t)port, "SignalK TCP", SK_TCP_HOST_PREF);
      } else if (strcmp(host_type, MPD_TCP_HOST_PREF) == 0) {
        String host = preferences.getString(MPD_TCP_HOST_PREF);
        int port = preferences.getInt(MPD_TCP_PORT_PREF);
        if (host.length() == 0 || host == "0.0.0.0") {
          host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(host.c_str(), (int32_t)port, "MPD Player", MPD_TCP_HOST_PREF);
      } else if (strcmp(host_type, NMEA0183_TCP_HOST_PREF) == 0) {
        String host = preferences.getString(NMEA0183_TCP_HOST_PREF);
        int port = preferences.getInt(NMEA0183_TCP_PORT_PREF);
        if (host.length() == 0 || host == "0.0.0.0") {
          host = WiFi.localIP().toString();
        }
        lv_ip_addr_editor_show(host.c_str(), (int32_t)port, "NMEA 0183 TCP", NMEA0183_TCP_HOST_PREF);
      }
    }
  }

  void lv_data_connections(lv_obj_t *parent) {
    data_connections_list = lv_list_create(parent);
    lv_obj_set_size(data_connections_list, 220, 180);
    lv_obj_align(data_connections_list, LV_ALIGN_CENTER, 0, 19);

    lv_obj_t *btn;
    btn = lv_list_add_btn(data_connections_list, NULL, "PyPilot TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)PYP_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "SignalK TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)SK_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "NMEA 0183 TCP");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)NMEA0183_TCP_HOST_PREF);
    btn = lv_list_add_btn(data_connections_list, NULL, "VE Venus MQTT");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)VENUS_MQTT_HOST_PREF);
#ifdef ENABLE_MPD 
    btn = lv_list_add_btn(data_connections_list, NULL, "MPD Player");
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)MPD_TCP_HOST_PREF);
#endif
    btn = lv_list_add_btn(data_connections_list, NULL, UI_BLANK_STR);
    lv_obj_add_event_cb(btn, event_handler_data_conn, LV_EVENT_CLICKED, (void *)UI_BLANK_STR);

    lv_ip_addr_editor(parent);
    lv_ip_addr_editor_hide();
  }

  static void erase_net_conf_evt_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_LONG_PRESSED) {
      erase_mdns_lookups();
      ESP_restart();
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
  static void lv_dev_status_display(lv_obj_t *parent) {
    pyp_status_label = lv_label_create(parent);
    lv_obj_align(pyp_status_label, LV_ALIGN_TOP_LEFT, 20, 30);
    lv_label_set_recolor(pyp_status_label, true);
    lv_label_set_text(pyp_status_label,
                      (String(F("#ff0000 " LV_SYMBOL_OK "  #PyPilot:              ")) += preferences.getString(PYP_TCP_HOST_PREF) += String(":") += String(preferences.getInt(PYP_TCP_PORT_PREF))).c_str());

    signalk_status_label = lv_label_create(parent);
    lv_obj_align(signalk_status_label, LV_ALIGN_TOP_LEFT, 20, 60);
    lv_label_set_recolor(signalk_status_label, true);
    lv_label_set_text(signalk_status_label,
                      (String(F("#ff0000 " LV_SYMBOL_OK "  #SignalK:             ")) += preferences.getString(SK_TCP_HOST_PREF) += String(":") += String(preferences.getInt(SK_TCP_PORT_PREF))).c_str());

    nmea0183_status_label = lv_label_create(parent);
    lv_obj_align(nmea0183_status_label, LV_ALIGN_TOP_LEFT, 20, 90);
    lv_label_set_recolor(nmea0183_status_label, true);
    lv_label_set_text(nmea0183_status_label,
                      (String(F("#ff0000 " LV_SYMBOL_OK "  #NMEA 0183:       ")) += preferences.getString(NMEA0183_TCP_HOST_PREF) += String(":") += String(preferences.getInt(NMEA0183_TCP_PORT_PREF))).c_str());

    venus_mqtt_status_label = lv_label_create(parent);
    lv_obj_align(venus_mqtt_status_label, LV_ALIGN_TOP_LEFT, 20, 120);
    lv_label_set_recolor(venus_mqtt_status_label, true);
    lv_label_set_text(venus_mqtt_status_label,
                      (String(F("#ff0000 " LV_SYMBOL_OK "  #Venus MQTT:    ")) += preferences.getString(VENUS_MQTT_HOST_PREF) += String(":") += String(preferences.getInt(VENUS_MQTT_PORT_PREF))).c_str());

#ifdef ENABLE_MPD // TODO:
    mpd_status_label = lv_label_create(parent);
    lv_obj_align(mpd_status_label, LV_ALIGN_TOP_LEFT, 20, 150);
    lv_label_set_recolor(mpd_status_label, true);
    lv_label_set_text(mpd_status_label,
                      (String(F("#ff0000 " LV_SYMBOL_OK "  #MPD:                  ")) += preferences.getString(MPD_TCP_HOST_PREF) += String(":") += String(preferences.getInt(MPD_TCP_PORT_PREF))).c_str());
#endif

    lv_obj_t *btn_erase = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_erase, erase_net_conf_evt_handler, LV_EVENT_ALL, parent);
    lv_obj_align(btn_erase, LV_ALIGN_CENTER, 60, 90);
    lv_obj_t *btn_erase_l = lv_label_create(btn_erase);
    lv_label_set_text_static(btn_erase_l, LV_SYMBOL_TRASH);
    lv_obj_set_style_bg_color(btn_erase, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    lv_obj_t *btn_edit = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_edit, edit_net_conf_evt_handler, LV_EVENT_ALL, parent);
    lv_obj_align(btn_edit, LV_ALIGN_CENTER, 0, 90);
    lv_obj_t *btn_edit_l = lv_label_create(btn_edit);
    lv_label_set_text_static(btn_edit_l, LV_SYMBOL_SETTINGS);

    lv_obj_t *btn_lcd = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_lcd, edit_lcd_conf_evt_handler, LV_EVENT_ALL, parent);
    lv_obj_align(btn_lcd, LV_ALIGN_CENTER, -60, 90);
    lv_obj_t *btn_lcd_l = lv_label_create(btn_lcd);
    lv_label_set_text_static(btn_lcd_l, LV_SYMBOL_EYE_OPEN);

    lv_data_connections(parent);
    lv_lcd_settings(parent);
    lv_obj_add_flag(data_connections_list, LV_OBJ_FLAG_HIDDEN);
  }

  static void dev_status_update_cb() {
    lv_label_set_text(pyp_status_label,
                      ((pypClient.c.connected() ? String(F("#00ff00 " LV_SYMBOL_OK "  #"))
                                                : String(F("#ff0000 " LV_SYMBOL_OK "  #")))
                       += String(F("PyPilot:              ")) += preferences.getString(PYP_TCP_HOST_PREF)
                       += String(":") += String(preferences.getInt(PYP_TCP_PORT_PREF)))
                        .c_str());
    lv_label_set_text(signalk_status_label,
                      ((skClient.c.connected() ? String(F("#00ff00 " LV_SYMBOL_OK "  #"))
                                               : String(F("#ff0000 " LV_SYMBOL_OK "  #")))
                       += String(F("SignalK:             ")) += preferences.getString(SK_TCP_HOST_PREF)
                       += String(":") += String(preferences.getInt(SK_TCP_PORT_PREF)))
                        .c_str());
    lv_label_set_text(nmea0183_status_label,
                      ((nmea0183Client.c.connected() ? String(F("#00ff00 " LV_SYMBOL_OK "  #"))
                                                     : String(F("#ff0000 " LV_SYMBOL_OK "  #")))
                       += String(F("NMEA 0183:       ")) += preferences.getString(NMEA0183_TCP_HOST_PREF)
                       += String(":") += String(preferences.getInt(NMEA0183_TCP_PORT_PREF)))
                        .c_str());
    lv_label_set_text(venus_mqtt_status_label,
                      ((mqttNetClient.connected() && mqttClient.connected() ? String(F("#00ff00 " LV_SYMBOL_OK "  #"))
                                                                            : String(F("#ff0000 " LV_SYMBOL_OK "  #")))
                       += String(F("Venus MQTT:    ")) += preferences.getString(VENUS_MQTT_HOST_PREF)
                       += String(":") += String(preferences.getInt(VENUS_MQTT_PORT_PREF)))
                        .c_str());
  }

  void init_devStatusScreen() {
    devStatusScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    devStatusScreen.init_cb = lv_dev_status_display;
    devStatusScreen.update_cb = dev_status_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

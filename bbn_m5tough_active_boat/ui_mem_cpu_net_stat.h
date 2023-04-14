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

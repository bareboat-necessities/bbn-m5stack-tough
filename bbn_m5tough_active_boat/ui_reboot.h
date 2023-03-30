#ifndef UI_REBOOT_H
#define UI_REBOOT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t rebootScreen;

  static void btnPowerOff_event(lv_event_t *event) {
    M5.Axp.PowerOff();
  }

  static void btnReboot_event(lv_event_t *event) {
    ESP.restart();
  }

  void init_rebootScreen() {
    rebootScreen.screen = lv_obj_create(NULL);  // Creates a Screen object

    lv_obj_t *labelIP = lv_label_create(rebootScreen.screen);
    lv_obj_set_pos(labelIP, 10, 10);
    lv_label_set_text(labelIP,
                      (" Wi-Fi:  " + wifi_ssid + "\n" + " Local IP:  " + WiFi.localIP().toString()).c_str());

    lv_obj_t *btn1 = lv_btn_create(rebootScreen.screen);
    lv_obj_t *label1 = lv_label_create(btn1);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label1, "Reboot Device");
    lv_obj_center(label1);
    lv_obj_add_event_cb(btn1, btnReboot_event, LV_EVENT_PRESSED, NULL);

    lv_obj_t *btn2 = lv_btn_create(rebootScreen.screen);
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 50);
    lv_label_set_text(label2, "Power Off");
    lv_obj_center(label2);
    lv_obj_add_event_cb(btn2, btnPowerOff_event, LV_EVENT_LONG_PRESSED, NULL);

    lv_obj_t *btn3 = lv_btn_create(rebootScreen.screen);
    lv_obj_t *label3 = lv_label_create(btn3);
    lv_obj_align(btn3, LV_ALIGN_CENTER, 0, -50);
    lv_label_set_text(label3, "Reset Wi-Fi Settings");
    lv_obj_center(label3);
    lv_obj_add_event_cb(btn3, btnResetWiFiSettings_event, LV_EVENT_LONG_PRESSED, NULL);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef UI_REBOOT_H
#define UI_REBOOT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t rebootScreen;

  static void btnPowerOff_event(lv_event_t *event) {
    disconnect_clients();
    M5.Axp.PowerOff();
  }

  static void btnReboot_event(lv_event_t *event) {
    ESP_restart();
  }

  static void lv_reboot_display(lv_obj_t *parent) {
    lv_obj_t *labelIP = lv_label_create(parent);
    lv_obj_set_pos(labelIP, 10, 10);
    lv_label_set_text(labelIP,
                      (String(" Wi-Fi:  ") += String(wifi_ssid) += String("\n Local IP:  ") += WiFi.localIP().toString()).c_str());

    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_t *label1 = lv_label_create(btn1);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 20);
    lv_label_set_text_static(label1, "Reboot");
    lv_obj_center(label1);
    lv_obj_add_event_cb(btn1, btnReboot_event, LV_EVENT_PRESSED, NULL);

    lv_obj_t *btn2 = lv_btn_create(parent);
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 80);
    lv_label_set_text_static(label2, LV_SYMBOL_POWER);
    lv_obj_center(label2);
    lv_obj_add_event_cb(btn2, btnPowerOff_event, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    lv_obj_t *btn3 = lv_btn_create(parent);
    lv_obj_t *label3 = lv_label_create(btn3);
    lv_obj_align(btn3, LV_ALIGN_CENTER, 0, -40);
    lv_label_set_text_static(label3, "Reset Wi-Fi");
    lv_obj_center(label3);
    lv_obj_add_event_cb(btn3, btnResetWiFiSettings_event, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_set_style_bg_color(btn3, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
  }

  void init_rebootScreen() {
    rebootScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    rebootScreen.init_cb = lv_reboot_display;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

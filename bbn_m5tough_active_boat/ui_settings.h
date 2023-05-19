#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

  static lv_obj_t *lcd_conf_obj;
  static lv_obj_t *lcd_slider_label;

  static void lv_lcd_slider_event_cb(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(lcd_slider_label, buf);
    lv_obj_align_to(lcd_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    set_lcd_backlight_voltage((int)lv_slider_get_value(slider));
  }

  static void btnRotateScreen_event(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
      preferences.end();
      preferences.begin("scr-cfg", false);
      bool rotate = preferences.getBool("ROTATE");
      if (rotate) {
        preferences.remove("ROTATE");
      } else {
        preferences.putBool("ROTATE", true);
      }
      preferences.end();
      ESP_restart();
    }
  }

  static void save_page(int page) {
    preferences.end();
    preferences.begin("scr-cfg", false);
    preferences.putInt("PAGE", page);
    preferences.end();
  }

  static int restore_page() {
    preferences.begin("scr-cfg", false);
    int page = preferences.getInt("PAGE");
    preferences.end();
    return page;
  }

  void lv_lcd_settings(lv_obj_t *parent) {
    lcd_conf_obj = lv_obj_create(parent);
    lv_obj_center(lcd_conf_obj);
    lv_obj_set_size(lcd_conf_obj, 300, 220);
    lv_obj_t *slider = lv_slider_create(lcd_conf_obj);
    lv_obj_center(slider);
    lv_obj_add_event_cb(slider, lv_lcd_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lcd_slider_label = lv_label_create(lcd_conf_obj);
    lv_slider_set_value(slider, 60, LV_ANIM_OFF);
    set_lcd_backlight_voltage((int)lv_slider_get_value(slider));
    lv_label_set_text_static(lcd_slider_label, "60%");

    lv_obj_align_to(lcd_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_add_flag(lcd_conf_obj, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *btn_rotate = lv_btn_create(lcd_conf_obj);
    lv_obj_t *label_rotate = lv_label_create(btn_rotate);
    lv_obj_align(btn_rotate, LV_ALIGN_CENTER, 0, 60);
    lv_label_set_text_static(label_rotate, LV_SYMBOL_LOOP);
    lv_obj_center(label_rotate);
    lv_obj_add_event_cb(btn_rotate, btnRotateScreen_event, LV_EVENT_CLICKED, NULL);
  }

  static void edit_lcd_conf_evt_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
      lv_obj_clear_flag(lcd_conf_obj, LV_OBJ_FLAG_HIDDEN);
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

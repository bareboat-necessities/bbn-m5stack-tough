#ifndef UI_IP_ADD_EDITOR_H
#define UI_IP_ADD_EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

  static lv_obj_t *spinboxes_parent;
  static lv_obj_t *spinbox1;
  static lv_obj_t *spinbox2;
  static lv_obj_t *spinbox3;
  static lv_obj_t *spinbox4;
  static lv_obj_t *spinbox_port;
  static lv_obj_t *ip_editor_label;

  static void lv_spinbox_port_inc_ev_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    if (spinbox != NULL) {
      if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox);
      }
    }
  }

  static void lv_spinbox_port_dec_ev_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    if (spinbox != NULL) {
      if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox);
      }
    }
  }

  lv_obj_t *lv_spinbox_port(lv_obj_t *parent, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    lv_obj_t *spinbox = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox, 0, 65535);
    lv_spinbox_set_digit_format(spinbox, 5, 0);
    lv_obj_set_width(spinbox, 80);
    lv_obj_align(spinbox, align, x_ofs, y_ofs);
    lv_obj_set_style_text_align(spinbox, LV_TEXT_ALIGN_CENTER, 0);

    lv_coord_t h = lv_obj_get_height(spinbox);
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_port_inc_ev_cb, LV_EVENT_ALL, (void *)spinbox);

    btn = lv_btn_create(parent);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_port_dec_ev_cb, LV_EVENT_ALL, (void *)spinbox);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);

    return spinbox;
  }

  static void lv_set_spinbox_val(lv_obj_t *spinbox, int32_t v) {
    int digits = 1;
    if (v > 99) {
      digits = 3;
    } else if (v > 9) {
      digits = 2;
    }
    ((lv_spinbox_t *)spinbox)->digit_count = digits;
    lv_spinbox_set_value(spinbox, v);
    ((lv_spinbox_t *)spinbox)->ta.cursor.show = 0;
  }

  static const char *editor_host_type;

  void lv_ip_addr_editor_show(const char *addr, int32_t port, const char *label, const char *host_type) {
    IPAddress ip;
    ip.fromString(addr);
    editor_host_type = host_type;

    lv_set_spinbox_val(spinbox1, ip[0]);
    lv_set_spinbox_val(spinbox2, ip[1]);
    lv_set_spinbox_val(spinbox3, ip[2]);
    lv_set_spinbox_val(spinbox4, ip[3]);
    lv_spinbox_set_value(spinbox_port, port);
    lv_spinbox_step_next(spinbox_port);
    lv_spinbox_step_next(spinbox_port);
    lv_spinbox_step_next(spinbox_port);
    lv_spinbox_step_next(spinbox_port);
    lv_label_set_text(ip_editor_label, label);

    lv_obj_clear_flag(spinboxes_parent, LV_OBJ_FLAG_HIDDEN);
#ifdef ENABLE_SCREEN_SERVER
    // (not for production)
    screenServer0();
#endif
  }

  static String editor_ip_address;

  void build_ip_address() {
    editor_ip_address = String(lv_spinbox_get_value(spinbox1));
    editor_ip_address += '.';
    editor_ip_address += String(lv_spinbox_get_value(spinbox2));
    editor_ip_address += '.';
    editor_ip_address += String(lv_spinbox_get_value(spinbox3));
    editor_ip_address += '.';
    editor_ip_address += String(lv_spinbox_get_value(spinbox4));
  }

  static int editor_port;

  void build_port() {
    editor_port = lv_spinbox_get_value(spinbox_port);
  }

  void lv_ip_addr_editor_hide() {
    lv_obj_add_flag(spinboxes_parent, LV_OBJ_FLAG_HIDDEN);
  }

  static void lv_spinbox_255_inc_ev_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    if (spinbox != NULL) {
      if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        int32_t v = lv_spinbox_get_value(spinbox);
        v = ((v + 1) >= 255 ? 0 : (v + 1));
        lv_set_spinbox_val(spinbox, v);
      }
      ((lv_spinbox_t *)spinbox)->ta.cursor.show = 0;
    }
  }

  static void lv_spinbox_255_dec_ev_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    if (spinbox != NULL) {
      if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        int32_t v = lv_spinbox_get_value(spinbox);
        v = ((v - 1) >= 0 ? (v - 1) : 255);
        lv_set_spinbox_val(spinbox, v);
      }
      ((lv_spinbox_t *)spinbox)->ta.cursor.show = 0;
    }
  }

  lv_obj_t *lv_spinbox_255(lv_obj_t *parent, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    lv_obj_t *spinbox = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox, 0, 255);
    lv_spinbox_set_digit_format(spinbox, 3, 0);
    lv_spinbox_set_step(spinbox, 1);
    lv_spinbox_set_rollover(spinbox, true);
    lv_obj_set_width(spinbox, 62);
    lv_obj_align(spinbox, align, x_ofs, y_ofs);
    lv_obj_set_style_text_align(spinbox, LV_TEXT_ALIGN_CENTER, 0);

    lv_coord_t h = lv_obj_get_height(spinbox);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_TOP_MID, 0, -10);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_255_inc_ev_cb, LV_EVENT_ALL, (void *)spinbox);

    btn = lv_btn_create(parent);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_255_dec_ev_cb, LV_EVENT_ALL, (void *)spinbox);

    lv_set_spinbox_val(spinbox, 0);
    return spinbox;
  }

  static void save_ip_evt_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
      build_ip_address();
      build_port();
      if (strcmp(editor_host_type, VENUS_MQTT_HOST_PREF) == 0) {
        preferences.putString(VENUS_MQTT_HOST_PREF, editor_ip_address);
        preferences.putInt(VENUS_MQTT_PORT_PREF, editor_port);
      } else if (strcmp(editor_host_type, PYP_TCP_HOST_PREF) == 0) {
        preferences.putString(PYP_TCP_HOST_PREF, editor_ip_address);
        preferences.putInt(PYP_TCP_PORT_PREF, editor_port);
      } else if (strcmp(editor_host_type, SK_TCP_HOST_PREF) == 0) {
        preferences.putString(SK_TCP_HOST_PREF, editor_ip_address);
        preferences.putInt(SK_TCP_PORT_PREF, editor_port);
      } else if (strcmp(editor_host_type, MPD_TCP_HOST_PREF) == 0) {
        preferences.putString(MPD_TCP_HOST_PREF, editor_ip_address);
        preferences.putInt(MPD_TCP_PORT_PREF, editor_port);
      } else if (strcmp(editor_host_type, NMEA0183_TCP_HOST_PREF) == 0) {
        preferences.putString(NMEA0183_TCP_HOST_PREF, editor_ip_address);
        preferences.putInt(NMEA0183_TCP_PORT_PREF, editor_port);
      }
      lv_ip_addr_editor_hide();
#ifdef ENABLE_SCREEN_SERVER
      // (not for production)
      screenServer0();
#endif
      ESP_restart();
    }
  }

  void lv_ip_addr_editor(lv_obj_t *parent) {
    spinboxes_parent = lv_obj_create(parent);
    lv_obj_set_size(spinboxes_parent, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    spinbox1 = lv_spinbox_255(spinboxes_parent, LV_ALIGN_CENTER, -105, -50);
    spinbox2 = lv_spinbox_255(spinboxes_parent, LV_ALIGN_CENTER, -35, -50);
    spinbox3 = lv_spinbox_255(spinboxes_parent, LV_ALIGN_CENTER, 35, -50);
    spinbox4 = lv_spinbox_255(spinboxes_parent, LV_ALIGN_CENTER, 105, -50);
    spinbox_port = lv_spinbox_port(spinboxes_parent, LV_ALIGN_CENTER, 0, 50);
    ip_editor_label = lv_label_create(spinboxes_parent);

    lv_set_spinbox_val(spinbox4, 0);
    lv_spinbox_set_value(spinbox_port, 0);

    lv_obj_t *btn_save = lv_btn_create(spinboxes_parent);
    lv_obj_add_event_cb(btn_save, save_ip_evt_handler, LV_EVENT_ALL, spinboxes_parent);
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_RIGHT, -4, -2);
    lv_obj_t *btn_save_l = lv_label_create(btn_save);
    lv_label_set_text_static(btn_save_l, LV_SYMBOL_OK);
    lv_obj_set_style_bg_color(btn_save, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);

    lv_obj_align(ip_editor_label, LV_ALIGN_BOTTOM_LEFT, 4, -8);
    lv_label_set_text_static(ip_editor_label, "");
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

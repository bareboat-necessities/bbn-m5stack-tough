#ifndef UI_IP_ADD_EDITOR_H
#define UI_IP_ADD_EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

  static lv_obj_t *spinbox1;
  static lv_obj_t *spinbox2;
  static lv_obj_t *spinbox3;
  static lv_obj_t *spinbox4;

  static void lv_set_spinbox_val(lv_obj_t *spinbox, int32_t v) {
    int digits = 1;
    if (v > 99) {
      digits = 3;
    } else if (v > 9) {
      digits = 2;
    }
    ((lv_spinbox_t *)spinbox)->digit_count = digits;
    lv_spinbox_set_value(spinbox, v);
  }

  void lv_ip_addr_editor_show(const char *addr) {
    IPAddress ip;
    ip.fromString(addr);

    lv_set_spinbox_val(spinbox1, ip[0]);
    lv_set_spinbox_val(spinbox2, ip[1]);
    lv_set_spinbox_val(spinbox3, ip[2]);
    lv_set_spinbox_val(spinbox4, ip[3]);

    lv_obj_clear_flag(spinbox1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(spinbox2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(spinbox3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(spinbox4, LV_OBJ_FLAG_HIDDEN);
  }

  void lv_ip_addr_editor_hide() {
    lv_obj_add_flag(spinbox1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(spinbox2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(spinbox3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(spinbox4, LV_OBJ_FLAG_HIDDEN);
  }

  lv_obj_t *lv_spinbox_255(lv_obj_t *parent, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    lv_obj_t *spinbox = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox, 0, 255);
    lv_spinbox_set_digit_format(spinbox, 3, 0);
    lv_spinbox_set_step(spinbox, 1);
    lv_spinbox_set_rollover(spinbox, true);
    lv_obj_set_width(spinbox, 62);
    lv_obj_align(spinbox, align, x_ofs, y_ofs);
    lv_obj_set_style_bg_opa(spinbox, 0, LV_PART_CURSOR);
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

  static void lv_spinbox_255_inc_ev_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    if (spinbox != NULL) {
      if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        int32_t v = lv_spinbox_get_value(spinbox);
        v = ((v + 1) >= 255 ? 0 : (v + 1));
        lv_set_spinbox_val(spinbox, v);
      }
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
    }
  }

  void lv_ip_addr_editor(lv_obj_t *parent, const char *addr) {
    spinbox1 = lv_spinbox_255(parent, LV_ALIGN_CENTER, -105, 0);
    spinbox2 = lv_spinbox_255(parent, LV_ALIGN_CENTER, -35, 0);
    spinbox3 = lv_spinbox_255(parent, LV_ALIGN_CENTER, 35, 0);
    spinbox4 = lv_spinbox_255(parent, LV_ALIGN_CENTER, 105, 0);

    IPAddress ip;
    ip.fromString(addr);
    lv_set_spinbox_val(spinbox1, ip[0]);
    lv_set_spinbox_val(spinbox2, ip[1]);
    lv_set_spinbox_val(spinbox3, ip[2]);
    lv_set_spinbox_val(spinbox4, ip[3]);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

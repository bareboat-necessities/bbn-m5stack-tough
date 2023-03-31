#ifndef UI_KEYBOARD_H
#define UI_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

  static lv_obj_t *kb;

  const lv_btnmatrix_ctrl_t ctrl_map[] = {
    4, 4, 4,
    4, 4, 4,
    4, 4, 4,
    3, 3, 3, 3
  };

  const char *btnm_mapplus[11][23] = {
    { "a", "b", "c", "\n",
      "d", "e", "f", "\n",
      "g", "h", "i", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "j", "k", "l", "\n",
      "n", "m", "o", "\n",
      "p", "q", "r", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "s", "t", "u", "\n",
      "v", "w", "x", "\n",
      "y", "z", " ", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "A", "B", "C", "\n",
      "D", "E", "F", "\n",
      "G", "H", "I", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "J", "K", "L", "\n",
      "N", "M", "O", "\n",
      "P", "Q", "R", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "S", "T", "U", "\n",
      "V", "W", "X", "\n",
      "Y", "Z", " ", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "1", "2", "3", "\n",
      "4", "5", "6", "\n",
      "7", "8", "9", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "0", "+", "-", "\n",
      "/", "*", "=", "\n",
      "!", "?", " ", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "<", ">", "@", "\n",
      "%", "$", "(", "\n",
      ")", "{", "}", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "[", "]", ";", "\n",
      "\"", "'", ".", "\n",
      ",", ":", " ", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" },
    { "\\", "_", "~", "\n",
      "|", "&", "^", "\n",
      "`", "#", " ", "\n",
      LV_SYMBOL_OK, LV_SYMBOL_BACKSPACE, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT, "" }
  };

  int keybd_index = 0;

  static void kb_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_target(e);
    if (kb != NULL) {
      if (code == LV_EVENT_VALUE_CHANGED) {
        uint16_t b = lv_keyboard_get_selected_btn(kb);
        const char *txt = lv_keyboard_get_btn_text(kb, b);
        if (txt != NULL) {
          if (strcmp(LV_SYMBOL_RIGHT, txt) == 0) {
            keybd_index = keybd_index + 1 >= sizeof(btnm_mapplus) / sizeof(btnm_mapplus[0]) ? 0 : keybd_index + 1;
            lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, btnm_mapplus[keybd_index], ctrl_map);
          } /*else if (strcmp(LV_SYMBOL_CLOSE, txt) == 0) {
          keybd_index = keybd_index - 1 < 0 ? 0 : keybd_index - 1;
          lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, btnm_mapplus[keybd_index], ctrl_map);
        }*/
        }
      }
    }
  }

  static void ta_keyboard_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_FOCUSED) {
      lv_keyboard_set_textarea(kb, ta);
      lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    } else if (code == LV_EVENT_DEFOCUSED) {
      lv_keyboard_set_textarea(kb, NULL);
      lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
  }

  void lv_keyboard2(lv_obj_t *parent) {
    // Create a keyboard to use it with all of the text areas
    kb = lv_keyboard_create(parent);
    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, btnm_mapplus[0], ctrl_map);
    lv_obj_set_height(kb, LV_VER_RES_MAX * 2 / 3);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL, NULL);

    // Create a text area. The keyboard will write here
    lv_obj_t *ta = lv_textarea_create(parent);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_size(ta, 240, 28);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_password_mode(ta, false);
    lv_textarea_set_text(ta, "");
    lv_obj_add_event_cb(ta, ta_keyboard_event_cb, LV_EVENT_ALL, kb);

    lv_keyboard_set_textarea(kb, ta);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

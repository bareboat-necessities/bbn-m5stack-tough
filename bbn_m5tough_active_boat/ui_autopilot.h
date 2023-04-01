#ifndef UI_AUTOPILOT_H
#define UI_AUTOPILOT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t autopilotScreen;

  static lv_obj_t *autopilot_led;
  static lv_obj_t *heading_l;
  static lv_obj_t *command_l;

  static const char *btnm_map[] = {
    LV_SYMBOL_DOUBLE_LEFT, LV_SYMBOL_DOUBLE_RIGHT, "\n",
    LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, "\n",
    "STANDBY", "AUTO", "\n",
    "MODE", LV_SYMBOL_EYE_OPEN, ""
  };

  void lv_autopilot_buttons(lv_obj_t *parent) {
    autopilot_led = lv_led_create(parent);
    lv_obj_align(autopilot_led, LV_ALIGN_OUT_LEFT_TOP, 8, 13);
    lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREY));
    lv_led_on(autopilot_led);

    int width_l = 134;
    int left_l = 52;
    int top_l = 14;

    heading_l = lv_label_create(parent);
    lv_label_set_text(heading_l, "HDG:  n/a");
    lv_obj_set_width(heading_l, width_l);
    lv_obj_set_style_text_align(heading_l, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(heading_l, LV_ALIGN_TOP_LEFT, left_l, top_l);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heading_l, &lv_font_montserrat_20, NULL);
#endif

    command_l = lv_label_create(parent);
    lv_label_set_text(command_l, "CMD:  n/a");
    lv_obj_set_width(command_l, width_l);
    lv_obj_set_style_text_align(command_l, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(command_l, LV_ALIGN_TOP_LEFT, left_l + width_l, top_l);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(command_l, &lv_font_montserrat_20, NULL);
#endif

    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_btnmatrix_set_btn_width(btnm, 6, 3);
    lv_obj_set_size(btnm, 320, 190);
    //lv_obj_add_event_cb(btnm, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 25);
  }

#define LONG_EXPIRE_TO 172800000

  static void autopilot_update_cb() {
    if (shipDataModel.steering.autopilot.ap_state.st == ap_state_e::STANDBY
        && fresh(shipDataModel.steering.autopilot.ap_state.age, LONG_EXPIRE_TO)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_RED));
    } else if (shipDataModel.steering.autopilot.ap_state.st == ap_state_e::ENGAGED
               && fresh(shipDataModel.steering.autopilot.ap_state.age, LONG_EXPIRE_TO)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREEN));
    }
    if (!fresh(shipDataModel.steering.autopilot.ap_state.age, LONG_EXPIRE_TO)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREY));
    }
    lv_label_set_text(heading_l,
                      ("HDG:  " + (fresh(shipDataModel.steering.autopilot.heading.age) ? String(shipDataModel.steering.autopilot.heading.deg, 0) + LV_SYMBOL_DEGREES : "n/a")).c_str());
    lv_label_set_text(command_l,
                      ("CMD:  " + (fresh(shipDataModel.steering.autopilot.command.age, LONG_EXPIRE_TO) ? String(shipDataModel.steering.autopilot.command.deg, 0) + LV_SYMBOL_DEGREES : "n/a")).c_str());
  }

  void init_autopilotScreen() {
    autopilotScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_autopilot_buttons(autopilotScreen.screen);
    autopilotScreen.update_cb = autopilot_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef UI_AUTOPILOT_H
#define UI_AUTOPILOT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t autopilotScreen;

  static lv_obj_t *autopilot_led;

  static const char *btnm_map[] = {
    LV_SYMBOL_DOUBLE_LEFT, LV_SYMBOL_DOUBLE_RIGHT, "\n",
    LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, "\n",
    "STANDBY", "AUTO", "\n",
    "MODE", LV_SYMBOL_EYE_OPEN, ""
  };

  void lv_autopilot_buttons(lv_obj_t *parent) {
    autopilot_led = lv_led_create(parent);
    lv_obj_align(autopilot_led, LV_ALIGN_OUT_LEFT_TOP, 16, 13);
    lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREY));
    lv_led_on(autopilot_led);

    int width_l = 128;
    int left_l = 64;
    int top_l = 14;

    lv_obj_t *heading = lv_label_create(parent);
    lv_label_set_text(heading, "HDG:  -120" LV_SYMBOL_DEGREES);
    lv_obj_set_width(heading, width_l);
    lv_obj_set_style_text_align(heading, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(heading, LV_ALIGN_TOP_LEFT, left_l, top_l);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heading, &lv_font_montserrat_20, NULL);
#endif

    lv_obj_t *command = lv_label_create(parent);
    lv_label_set_text(command, "CMD:  -120" LV_SYMBOL_DEGREES);
    lv_obj_set_width(command, width_l);
    lv_obj_set_style_text_align(command, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(command, LV_ALIGN_TOP_LEFT, left_l + width_l, top_l);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(command, &lv_font_montserrat_20, NULL);
#endif

    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_btnmatrix_set_btn_width(btnm, 6, 3);
    lv_obj_set_size(btnm, 320, 190);
    //lv_obj_add_event_cb(btnm, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 25);
  }

  static void autopilot_update_cb() {
    if (shipDataModel.steering.autopilot.ap_state.st == ap_state_e::STANDBY
        && fresh(shipDataModel.steering.autopilot.ap_state.age, TWO_MINUTES)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_RED));
    } else if (shipDataModel.steering.autopilot.ap_state.st == ap_state_e::ENGAGED
               && fresh(shipDataModel.steering.autopilot.ap_state.age, TWO_MINUTES)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREEN));
    }
    if (!fresh(shipDataModel.steering.autopilot.ap_state.age, TWO_MINUTES)) {
      lv_led_set_color(autopilot_led, lv_palette_main(LV_PALETTE_GREY));
    }
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

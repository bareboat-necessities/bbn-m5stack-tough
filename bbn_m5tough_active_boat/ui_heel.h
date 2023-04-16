#ifndef UI_HEEL_H
#define UI_HEEL_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t heelScreen;

  static lv_obj_t *heel_display;
  static lv_meter_indicator_t *indic_heel;
  static lv_obj_t *pitch_label;

  static void set_heel_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(heel_display, (lv_meter_indicator_t *)indic, v);
  }

  /**
   * A heel display (clinometer)
   */
  void lv_heel_display(lv_obj_t *parent) {
    heel_display = lv_meter_create(parent);
    lv_obj_align(heel_display, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_size(heel_display, 300, 300);
    //v_obj_set_style_arc_width(heel_display, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(heel_display, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(heel_display, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(heel_display, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);

    /*Add a scale first*/
    lv_meter_scale_t *scale = lv_meter_add_scale(heel_display);
    lv_meter_set_scale_range(heel_display, scale, -45, 45, 100, 40);
    lv_meter_set_scale_ticks(heel_display, scale, 19, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(heel_display, scale, 3, 3, 20, lv_palette_main(LV_PALETTE_GREY), 15);

    /*Add a needle line indicator*/
    indic_heel = lv_meter_add_needle_line(heel_display, scale, 7, lv_palette_main(LV_PALETTE_ORANGE), -10);

    pitch_label = lv_label_create(parent);
    lv_obj_align(pitch_label, LV_ALIGN_TOP_LEFT, 5, 5);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(pitch_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(pitch_label, "PITCH:   --");

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text(main_label, "HEEL");
  }

  static void heel_update_cb() {
    lv_label_set_text(pitch_label,
                      (String("PITCH:   ")
                       + (fresh(shipDataModel.navigation.attitude.pitch.age) ? String(shipDataModel.navigation.attitude.pitch.deg) + String(LV_SYMBOL_DEGREES) : String("--")))
                        .c_str());

    set_heel_value(indic_heel, fresh(shipDataModel.navigation.attitude.heel.age) ? shipDataModel.navigation.attitude.heel.deg : 0);
  }

  void init_heelScreen() {
    heelScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_heel_display(heelScreen.screen);
    heelScreen.update_cb = heel_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

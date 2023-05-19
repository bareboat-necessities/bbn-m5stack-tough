#ifndef UI_HEEL_H
#define UI_HEEL_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t heelScreen;

  static lv_obj_t *heel_display;
  static lv_meter_indicator_t *indic_heel;
  static lv_obj_t *pitch_label;
  static lv_obj_t *heel_leeway_label;
  static lv_obj_t *heel_drift_label;
  static lv_obj_t *heel_set_label;

  static void set_heel_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(heel_display, (lv_meter_indicator_t *)indic, v);
  }

  /**
   * A heel display (clinometer)
   */
  static void lv_heel_display(lv_obj_t *parent) {
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
    lv_label_set_text_static(pitch_label, "Pitch:   --");

    heel_leeway_label = lv_label_create(parent);
    lv_obj_align(heel_leeway_label, LV_ALIGN_TOP_LEFT, 5, 50);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heel_leeway_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(heel_leeway_label, "Leeway\n(est):\n--");

    heel_drift_label = lv_label_create(parent);
    lv_obj_align(heel_drift_label, LV_ALIGN_TOP_LEFT, 175, 5);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heel_drift_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(heel_drift_label, "Drift (kt):  --");

    heel_set_label = lv_label_create(parent);
    lv_obj_align(heel_set_label, LV_ALIGN_TOP_LEFT, 245, 50);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heel_set_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(heel_set_label, "Set:\n--");

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text_static(main_label, "HEEL");
  }

  static void heel_update_cb() {
    lv_label_set_text(pitch_label,
                      (String("Pitch:   ") += (fresh(shipDataModel.navigation.attitude.pitch.age)
                                                 ? String(shipDataModel.navigation.attitude.pitch.deg) += LV_SYMBOL_DEGREES
                                                 : String("--")))
                        .c_str());
    lv_label_set_text(heel_leeway_label,
                      ((String("Leeway\n(est):\n") += (fresh(shipDataModel.navigation.leeway.age)
                                                         ? String(shipDataModel.navigation.leeway.deg, 1)
                                                         : String("--"))) += LV_SYMBOL_DEGREES)
                        .c_str());
    lv_label_set_text(heel_drift_label,
                      (String("Drift (kt):  ") += (fresh(shipDataModel.navigation.drift.age)
                                                   ? String(shipDataModel.navigation.drift.kn, 1)
                                                   : String("--")))
                        .c_str());
    lv_label_set_text(heel_set_label,
                      ((String("Set:\n") += (fresh(shipDataModel.navigation.set_true.age)
                                               ? String(shipDataModel.navigation.set_true.deg, 0)
                                               : String("--"))) += (LV_SYMBOL_DEGREES "t"))
                        .c_str());
    set_heel_value(indic_heel, fresh(shipDataModel.navigation.attitude.heel.age)
                                 ? shipDataModel.navigation.attitude.heel.deg
                                 : 0);
  }

  void init_heelScreen() {
    heelScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    heelScreen.init_cb = lv_heel_display;
    heelScreen.update_cb = heel_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

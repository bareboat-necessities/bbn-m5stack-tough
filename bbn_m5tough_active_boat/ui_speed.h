#ifndef UI_SPEED_H
#define UI_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t speedScreen;

  static lv_obj_t *sog_label;
  static lv_obj_t *sog_avg_label;
  static lv_obj_t *spd_label;
  static lv_obj_t *leeway_label;
  static lv_obj_t *s_cogt_label;
  static lv_obj_t *s_hdt_label;

  /**
   * A speed display 
   */
  static void lv_speed_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_recolor(main_label, true);
    lv_label_set_text_static(main_label, "SPEED  #0080ff " LV_SYMBOL_UPLOAD " #");

    sog_label = lv_label_create(parent);
    lv_obj_align(sog_label, LV_ALIGN_TOP_LEFT, 10, 35);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(sog_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(sog_label, "SOG (kt):                       --");

    sog_avg_label = lv_label_create(parent);
    lv_obj_align(sog_avg_label, LV_ALIGN_TOP_LEFT, 10, 70);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(sog_avg_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(sog_avg_label, "SOG AVG (kt):             --");

    spd_label = lv_label_create(parent);
    lv_obj_align(spd_label, LV_ALIGN_TOP_LEFT, 10, 105);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(spd_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(spd_label, "SPD (kt):                       --");

    leeway_label = lv_label_create(parent);
    lv_obj_align(leeway_label, LV_ALIGN_TOP_LEFT, 10, 140);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(leeway_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(leeway_label, "Leeway (est):              --" LV_SYMBOL_DEGREES);

    s_cogt_label = lv_label_create(parent);
    lv_obj_align(s_cogt_label, LV_ALIGN_TOP_LEFT, 10, 175);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(s_cogt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(s_cogt_label, "COGT:                            --");

    s_hdt_label = lv_label_create(parent);
    lv_obj_align(s_hdt_label, LV_ALIGN_TOP_LEFT, 10, 210);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(s_hdt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(s_hdt_label, "HDT:                               --");
  }

  static void speed_update_cb() {
    lv_label_set_text(sog_label,
                      (String("SOG (kt):                       ") += (fresh(shipDataModel.navigation.speed_over_ground.age) ? String(shipDataModel.navigation.speed_over_ground.kn, 1) : String("--")))
                        .c_str());
    lv_label_set_text(sog_avg_label,
                      (String("SOG AVG (kt):             ") += (fresh(shipDataModel.navigation.speed_over_ground_avg.age, 20000) ? String(shipDataModel.navigation.speed_over_ground_avg.kn, 1) : String("--")))
                        .c_str());
    lv_label_set_text(spd_label,
                      (String("SPD (kt):                       ") += (fresh(shipDataModel.navigation.speed_through_water.age) ? String(shipDataModel.navigation.speed_through_water.kn, 1) : String("--")))
                        .c_str());
    lv_label_set_text(leeway_label,
                      (String("Leeway (est):              ") += (fresh(shipDataModel.navigation.leeway.age) ? String(shipDataModel.navigation.leeway.deg, 1) += LV_SYMBOL_DEGREES : String("--")))
                        .c_str());
    lv_label_set_text(s_cogt_label,
                      (String("COGT:                            ") += (fresh(shipDataModel.navigation.course_over_ground_true.age) ? String(shipDataModel.navigation.course_over_ground_true.deg, 1) += LV_SYMBOL_DEGREES : String("--")))
                        .c_str());
    lv_label_set_text(s_hdt_label,
                      (String("HDT:                               ") += (fresh(shipDataModel.navigation.heading_true.age) ? String(shipDataModel.navigation.heading_true.deg, 1) += LV_SYMBOL_DEGREES : String("--")))
                        .c_str());
  }

  void init_speedScreen() {
    speedScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    speedScreen.init_cb = lv_speed_display;
    speedScreen.update_cb = speed_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

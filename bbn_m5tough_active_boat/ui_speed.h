#ifndef UI_SPEED_H
#define UI_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t speedScreen;

  static lv_obj_t *sog_label;
  static lv_obj_t *sog_avg_label;
  static lv_obj_t *spd_label;
  static lv_obj_t *g_force_label;

  /**
   * A speed display 
   */
  void lv_speed_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_text(main_label, "SPEED  " LV_SYMBOL_UPLOAD);

    sog_label = lv_label_create(parent);
    lv_obj_align(sog_label, LV_ALIGN_TOP_LEFT, 10, 40);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(sog_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(sog_label, "SOG (kt):                       --");

    sog_avg_label = lv_label_create(parent);
    lv_obj_align(sog_avg_label, LV_ALIGN_TOP_LEFT, 10, 80);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(sog_avg_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(sog_avg_label, "SOG AVG (kt):             --");

    spd_label = lv_label_create(parent);
    lv_obj_align(spd_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(spd_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(spd_label, "SPD (kt):                       --");

    g_force_label = lv_label_create(parent);
    lv_obj_align(g_force_label, LV_ALIGN_TOP_LEFT, 10, 160);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(g_force_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(g_force_label, "G Force (% of g):         --");
  }

  static void speed_update_cb() {
  }

  void init_speedScreen() {
    speedScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_speed_display(speedScreen.screen);
    speedScreen.update_cb = speed_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

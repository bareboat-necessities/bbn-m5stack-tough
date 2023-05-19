#ifndef UI_RUDDER_H
#define UI_RUDDER_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t rudderScreen;

  static lv_obj_t *rudder_display;
  static lv_meter_indicator_t *indic_rudder;
  static lv_obj_t *rate_of_turn_label;
  static lv_obj_t *rudder_hdt_label;

  static void set_rudder_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(rudder_display, (lv_meter_indicator_t *)indic, v);
  }

  /**
   * A rudder position display
   */
  static void lv_rudder_display(lv_obj_t *parent) {
    rudder_display = lv_meter_create(parent);
    lv_obj_align(rudder_display, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_size(rudder_display, 300, 300);
    //v_obj_set_style_arc_width(rudder_display, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(rudder_display, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rudder_display, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(rudder_display, lv_theme_get_color_primary(parent), LV_PART_INDICATOR);

    /*Add a scale first*/
    lv_meter_scale_t *scale = lv_meter_add_scale(rudder_display);
    lv_meter_set_scale_range(rudder_display, scale, -45, 45, 100, 40);
    //lv_linemeter_set_mirror(rudder_display, true);
    lv_meter_set_scale_ticks(rudder_display, scale, 19, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(rudder_display, scale, 3, 3, 20, lv_palette_main(LV_PALETTE_GREY), 15);

    /*Add a needle line indicator*/
    indic_rudder = lv_meter_add_needle_line(rudder_display, scale, 7, lv_theme_get_color_primary(parent), -10);

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text_static(main_label, "RUDDER");

    rate_of_turn_label = lv_label_create(parent);
    lv_obj_align(rate_of_turn_label, LV_ALIGN_TOP_LEFT, 5, 5);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(rate_of_turn_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(rate_of_turn_label, "ROT (" LV_SYMBOL_DEGREES "/min):   --");

    rudder_hdt_label = lv_label_create(parent);
    lv_obj_align(rudder_hdt_label, LV_ALIGN_TOP_LEFT, 5, 50);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(rudder_hdt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(rudder_hdt_label, "HDT:\n--");
  }

  static void rudder_update_cb() {
    lv_label_set_text(rate_of_turn_label,
                      (String("ROT (" LV_SYMBOL_DEGREES "/min):   ") += (fresh(shipDataModel.navigation.rate_of_turn.age) 
                      ? String(shipDataModel.navigation.rate_of_turn.deg_min) : String("--"))).c_str());
    lv_label_set_text(rudder_hdt_label,
                      (fresh(shipDataModel.navigation.heading_true.age) 
                      ? String("HDT:\n") += String(shipDataModel.navigation.heading_true.deg, 0) += LV_SYMBOL_DEGREES 
                      : "HDT:\n--").c_str());

    set_rudder_value(indic_rudder, fresh(shipDataModel.steering.rudder_angle.age) ? shipDataModel.steering.rudder_angle.deg : 0);
  }

  void init_rudderScreen() {
    rudderScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    rudderScreen.init_cb = lv_rudder_display;
    rudderScreen.update_cb = rudder_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

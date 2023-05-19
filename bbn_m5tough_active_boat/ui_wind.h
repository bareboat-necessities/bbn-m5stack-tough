#ifndef UI_WIND_H
#define UI_WIND_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t windScreen;

  static lv_obj_t *wind_display;
  static lv_meter_indicator_t *indic_wind;
  static lv_meter_indicator_t *indic_gwa_wind;
  static lv_obj_t *wind_label;
  static lv_obj_t *spd_w_label;
  static lv_obj_t *gws_label;
  static lv_obj_t *gwdt_label;

  static void set_wind_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(wind_display, (lv_meter_indicator_t *)indic, v);
  }

  /**
 * A wind display meter
 */
  static void lv_wind_display(lv_obj_t *parent) {
    wind_display = lv_meter_create(parent);
    lv_obj_align(wind_display, LV_ALIGN_CENTER, 0, 6);
    lv_obj_set_size(wind_display, 210, 210);

    /*Add a scale first*/
    lv_meter_scale_t *scale = lv_meter_add_scale(wind_display);
    lv_meter_set_scale_ticks(wind_display, scale, 37, 2, 9, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(wind_display, scale, -180, 180, 360, 90);

    lv_meter_scale_t *scale2 = lv_meter_add_scale(wind_display);
    lv_meter_set_scale_ticks(wind_display, scale2, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(wind_display, scale2, 1, 3, 14, lv_palette_main(LV_PALETTE_GREY), 14); /*Every tick is major*/
    lv_meter_set_scale_range(wind_display, scale2, -150, 180, 330, 120);

    /*Add a red arc to the start*/
    indic_wind = lv_meter_add_arc(wind_display, scale, 4, lv_palette_main(LV_PALETTE_RED), 2);
    lv_meter_set_indicator_start_value(wind_display, indic_wind, -60);
    lv_meter_set_indicator_end_value(wind_display, indic_wind, -20);

    /*Make the tick lines red at the start of the scale*/
    indic_wind = lv_meter_add_scale_lines(
      wind_display, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(wind_display, indic_wind, -60);
    lv_meter_set_indicator_end_value(wind_display, indic_wind, -20);

    /*Add a green arc to the end*/
    indic_wind = lv_meter_add_arc(wind_display, scale, 4, lv_palette_main(LV_PALETTE_GREEN), 2);
    lv_meter_set_indicator_start_value(wind_display, indic_wind, 20);
    lv_meter_set_indicator_end_value(wind_display, indic_wind, 60);

    /*Make the tick lines green at the end of the scale*/
    indic_wind = lv_meter_add_scale_lines(
      wind_display, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    lv_meter_set_indicator_start_value(wind_display, indic_wind, 20);
    lv_meter_set_indicator_end_value(wind_display, indic_wind, 60);

    /*Add a needle line indicator*/
    indic_wind = lv_meter_add_needle_line(wind_display, scale, 6, lv_palette_main(LV_PALETTE_GREY), -10);

    /*Add a ground wind angle needle line indicator*/
    indic_gwa_wind = lv_meter_add_needle_line(wind_display, scale, 6, lv_palette_main(LV_PALETTE_ORANGE), -48);

    wind_label = lv_label_create(parent);
    lv_obj_align(wind_label, LV_ALIGN_TOP_LEFT, 5, 2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(wind_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(wind_label, "AWS:  --\nkt");

    spd_w_label = lv_label_create(parent);
    lv_obj_align(spd_w_label, LV_ALIGN_TOP_RIGHT, -5, 2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(spd_w_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(spd_w_label, "SPD:  --\n      kt");

    gws_label = lv_label_create(parent);
    lv_obj_align(gws_label, LV_ALIGN_BOTTOM_LEFT, 5, -2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(gws_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(gws_label, "GWS:\n-- kt");

    gwdt_label = lv_label_create(parent);
    lv_obj_align(gwdt_label, LV_ALIGN_BOTTOM_RIGHT, -5, -2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(gwdt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(gwdt_label, "GWD:\n--" LV_SYMBOL_DEGREES "t");
  }

  static void wind_update_cb() {
    lv_label_set_text(wind_label,
                      (String("AWS:   ") += (fresh(shipDataModel.environment.wind.apparent_wind_speed.age) ? String(shipDataModel.environment.wind.apparent_wind_speed.kn, 1) += "\nkt" : String("--\nkt")))
                        .c_str());
    lv_label_set_text(spd_w_label,
                      (String("SPD:  ") += (fresh(shipDataModel.navigation.speed_through_water.age) ? String(shipDataModel.navigation.speed_through_water.kn, 1) += "\n     kt" : String("--\n     kt")))
                        .c_str());
    lv_label_set_text(gws_label,
                      (String("GWS:\n") += (fresh(shipDataModel.environment.wind.ground_wind_speed.age) ? String(shipDataModel.environment.wind.ground_wind_speed.kn, 1) += " kt" : String("-- kt")))
                        .c_str());
    lv_label_set_text(gwdt_label,
                      (String("GWD:\n") += (fresh(shipDataModel.environment.wind.ground_wind_dir_true.age) ? String(shipDataModel.environment.wind.ground_wind_dir_true.deg, 0) += LV_SYMBOL_DEGREES "t" : String("--" LV_SYMBOL_DEGREES "t")))
                        .c_str());
    set_wind_value(indic_wind, fresh(shipDataModel.environment.wind.apparent_wind_angle.age) ? shipDataModel.environment.wind.apparent_wind_angle.deg : 0);
    set_wind_value(indic_gwa_wind, fresh(shipDataModel.environment.wind.ground_wind_angle.age) ? shipDataModel.environment.wind.ground_wind_angle.deg : 0);
  }

  void init_windScreen() {
    windScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    windScreen.init_cb = lv_wind_display;
    windScreen.update_cb = wind_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

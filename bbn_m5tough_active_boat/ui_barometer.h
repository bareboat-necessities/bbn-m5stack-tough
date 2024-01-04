
#ifndef UI_BAROMETER_H
#define UI_BAROMETER_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t barometerScreen;

  static lv_obj_t *barometer_display;
  static lv_meter_indicator_t *indic_barometer;
  static lv_obj_t *pressure_label;
  static lv_obj_t *temperature_label;
  static lv_obj_t *bar;

  static void set_pressure_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(barometer_display, (lv_meter_indicator_t *)indic, v);
  }

  static void set_temperature_value(int32_t v) {
    lv_bar_set_value(bar, v, LV_ANIM_OFF);
  }

  /**
 * A pressure display meter
 */
  static void lv_barometer_display(lv_obj_t *parent) {
    barometer_display = lv_meter_create(parent);
    lv_obj_align(barometer_display, LV_ALIGN_CENTER, -40, 0);
    lv_obj_set_size(barometer_display, 193, 193);

    lv_meter_scale_t *scale = lv_meter_add_scale(barometer_display);
    lv_meter_set_scale_ticks(barometer_display, scale, 26, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(barometer_display, scale, 5, 4, 15, lv_palette_main(LV_PALETTE_GREY), 20);
    lv_meter_set_scale_range(barometer_display, scale, 960, 1060, 240, 150);

    /*Add a blue arc to the start*/
    indic_barometer = lv_meter_add_arc(barometer_display, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(barometer_display, indic_barometer, 900);
    lv_meter_set_indicator_end_value(barometer_display, indic_barometer, 940);

    /*Make the tick lines blue at the start of the scale*/
    indic_barometer = lv_meter_add_scale_lines(barometer_display, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(barometer_display, indic_barometer, 960);
    lv_meter_set_indicator_end_value(barometer_display, indic_barometer, 993);

    /*Make the tick lines blue at the start of the scale*/
    indic_barometer = lv_meter_add_scale_lines(barometer_display, scale, lv_palette_main(LV_PALETTE_YELLOW), lv_palette_main(LV_PALETTE_YELLOW), false, 0);
    lv_meter_set_indicator_start_value(barometer_display, indic_barometer, 994);
    lv_meter_set_indicator_end_value(barometer_display, indic_barometer, 1027);

    /*Make the tick lines blue at the start of the scale*/
    indic_barometer = lv_meter_add_scale_lines(barometer_display, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    lv_meter_set_indicator_start_value(barometer_display, indic_barometer, 1028);
    lv_meter_set_indicator_end_value(barometer_display, indic_barometer, 1060);

    /*Add a needle line indicator*/
    indic_barometer = lv_meter_add_needle_line(barometer_display, scale, 4, lv_palette_main(LV_PALETTE_GREY), 0);

    pressure_label = lv_label_create(barometer_display);
    lv_obj_align(pressure_label, LV_ALIGN_CENTER, 0, 60);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(pressure_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(pressure_label, "\n  hPa");

    /*Thermometer*/

    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    bar = lv_bar_create(parent);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 30, 145);
    lv_obj_align(bar, LV_ALIGN_CENTER, 107, -25);
    lv_bar_set_range(bar, 0, 40);

    temperature_label = lv_label_create(parent);
    lv_obj_align(temperature_label, LV_ALIGN_CENTER, 107, 70);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(temperature_label, "  C");
  }

  static void pressure_update_cb() {
    lv_label_set_text(pressure_label,
                      ((fresh(shipDataModel.environment.air_outside.pressure.age) ? String(shipDataModel.environment.air_outside.pressure.hPa, 1) += "\n  hPa" : String("--\n  hPa")))
                        .c_str());

    set_pressure_value(indic_barometer, fresh(shipDataModel.environment.air_outside.pressure.age) ? shipDataModel.environment.air_outside.pressure.hPa : 0);

    set_temperature_value(fresh(shipDataModel.environment.air_outside.temp_deg_C.age) ? shipDataModel.environment.air_outside.temp_deg_C.deg_C : 0);

     lv_label_set_text(temperature_label,
                      ((fresh(shipDataModel.environment.air_outside.temp_deg_C.age) ? String(shipDataModel.environment.air_outside.temp_deg_C.deg_C, 1) += "  C" : String("--  C")))
                        .c_str());
  }

  void init_barometerScreen() {
    barometerScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    barometerScreen.init_cb = lv_barometer_display;
    barometerScreen.update_cb = pressure_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif


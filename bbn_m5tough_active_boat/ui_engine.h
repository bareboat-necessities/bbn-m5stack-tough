#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t engineScreen;

  static lv_obj_t *engine_rpm_meter;
  static lv_meter_indicator_t *engine_rpm_indic;

  static void set_engine_rpm_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(engine_rpm_meter, (lv_meter_indicator_t *)indic, v);
  }

  /**
   * A engine display
   */
  static void lv_engine_display(lv_obj_t *parent) {
    engine_rpm_meter = lv_meter_create(parent);
    lv_obj_center(engine_rpm_meter);
    lv_obj_set_size(engine_rpm_meter, 200, 200);

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 50);
    lv_label_set_text(main_label, "RPM\nx100");

    /*Add a scale first*/
    lv_meter_scale_t *scale = lv_meter_add_scale(engine_rpm_meter);
    lv_meter_set_scale_ticks(engine_rpm_meter, scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(engine_rpm_meter, scale, 5, 4, 15, lv_palette_main(LV_PALETTE_GREY), 10);
    lv_meter_set_scale_range(engine_rpm_meter, scale, 0, 60, 240, 150);

    /*Add a blue arc to the start*/
    engine_rpm_indic = lv_meter_add_arc(engine_rpm_meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(engine_rpm_meter, engine_rpm_indic, 0);
    lv_meter_set_indicator_end_value(engine_rpm_meter, engine_rpm_indic, 20);

    /*Make the tick lines blue at the start of the scale*/
    engine_rpm_indic = lv_meter_add_scale_lines(
      engine_rpm_meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), true, 0);
    lv_meter_set_indicator_start_value(engine_rpm_meter, engine_rpm_indic, 0);
    lv_meter_set_indicator_end_value(engine_rpm_meter, engine_rpm_indic, 20);

    /*Add a red arc to the end*/
    engine_rpm_indic = lv_meter_add_arc(engine_rpm_meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(engine_rpm_meter, engine_rpm_indic, 40);
    lv_meter_set_indicator_end_value(engine_rpm_meter, engine_rpm_indic, 60);

    /*Make the tick lines red at the end of the scale*/
    engine_rpm_indic = lv_meter_add_scale_lines(
      engine_rpm_meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), true, 0);
    lv_meter_set_indicator_start_value(engine_rpm_meter, engine_rpm_indic, 40);
    lv_meter_set_indicator_end_value(engine_rpm_meter, engine_rpm_indic, 60);

    /*Add a needle line indicator*/
    engine_rpm_indic = lv_meter_add_needle_line(engine_rpm_meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
  }

  static void engine_update_cb() {
    set_engine_rpm_value(engine_rpm_indic,
                         (fresh(shipDataModel.propulsion.engines[0].revolutions_RPM.age)
                            ? shipDataModel.propulsion.engines[0].revolutions_RPM.rpm * 100
                            : 0));
  }

  void init_engineScreen() {
    engineScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    engineScreen.init_cb = lv_engine_display;
    engineScreen.update_cb = engine_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

  static lv_obj_t *engine_rpm_meter;

static void set_engine_rpm_value(void *indic, int32_t v) {
  lv_meter_set_indicator_value(engine_rpm_meter, (lv_meter_indicator_t *)indic, v);
}

/**
 * A simple meter
 */
void lv_engine_rpm_meter(lv_obj_t *parent) {
  engine_rpm_meter = lv_meter_create(parent);
  lv_obj_center(engine_rpm_meter);
  lv_obj_set_size(engine_rpm_meter, 200, 200);

  /*Add a scale first*/
  lv_meter_scale_t *scale = lv_meter_add_scale(engine_rpm_meter);
  lv_meter_set_scale_ticks(engine_rpm_meter, scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(engine_rpm_meter, scale, 5, 4, 15, lv_palette_main(LV_PALETTE_GREY), 10);
  lv_meter_set_scale_range(engine_rpm_meter, scale, 0, 60, 240, 150);

  lv_meter_indicator_t *engine_rpm_indic;

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

  /*Create an animation to set the value*/
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, set_engine_rpm_value);
  lv_anim_set_var(&a, engine_rpm_indic);
  lv_anim_set_values(&a, 0, 60);
  lv_anim_set_time(&a, 2000);
  lv_anim_set_repeat_delay(&a, 100);
  lv_anim_set_playback_time(&a, 500);
  lv_anim_set_playback_delay(&a, 100);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);
}

  
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

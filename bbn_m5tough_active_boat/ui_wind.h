#ifndef UI_WIND_H
#define UI_WIND_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t windScreen;

  static lv_obj_t *wind_display;

  static void set_wind_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(wind_display, (lv_meter_indicator_t *)indic, v);
  }

  /**
 * A wind display meter
 */
  void lv_wind_display(lv_obj_t *parent) {
    wind_display = lv_meter_create(parent);
    lv_obj_center(wind_display);
    lv_obj_set_size(wind_display, 210, 210);

    /*Add a scale first*/
    lv_meter_scale_t *scale = lv_meter_add_scale(wind_display);
    lv_meter_set_scale_ticks(wind_display, scale, 37, 2, 9, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(wind_display, scale, -180, 180, 360, 90);

    lv_meter_scale_t *scale2 = lv_meter_add_scale(wind_display);
    lv_meter_set_scale_ticks(wind_display, scale2, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(wind_display, scale2, 1, 3, 14, lv_palette_main(LV_PALETTE_GREY), 14); /*Every tick is major*/
    lv_meter_set_scale_range(wind_display, scale2, -150, 180, 330, 120);

    lv_meter_indicator_t *indic;

    /*Add a red arc to the start*/
    indic = lv_meter_add_arc(wind_display, scale, 4, lv_palette_main(LV_PALETTE_RED), 2);
    lv_meter_set_indicator_start_value(wind_display, indic, -60);
    lv_meter_set_indicator_end_value(wind_display, indic, -20);

    /*Make the tick lines red at the start of the scale*/
    indic = lv_meter_add_scale_lines(
      wind_display, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(wind_display, indic, -60);
    lv_meter_set_indicator_end_value(wind_display, indic, -20);

    /*Add a green arc to the end*/
    indic = lv_meter_add_arc(wind_display, scale, 4, lv_palette_main(LV_PALETTE_GREEN), 2);
    lv_meter_set_indicator_start_value(wind_display, indic, 20);
    lv_meter_set_indicator_end_value(wind_display, indic, 60);

    /*Make the tick lines green at the end of the scale*/
    indic = lv_meter_add_scale_lines(
      wind_display, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    lv_meter_set_indicator_start_value(wind_display, indic, 20);
    lv_meter_set_indicator_end_value(wind_display, indic, 60);

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(wind_display, scale, 6, lv_palette_main(LV_PALETTE_GREY), -10);

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_wind_value);
    lv_anim_set_var(&a, indic);
    lv_anim_set_values(&a, -60, -20);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
  }

  static void wind_update_cb() {
    // TODO:
  }

  void init_windScreen() {
    windScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_wind_display(windScreen.screen);
    windScreen.update_cb = wind_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

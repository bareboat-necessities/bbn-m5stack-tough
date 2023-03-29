#ifndef UI_HEEL_H
#define UI_HEEL_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t heelScreen;

  static lv_obj_t *heel_display;

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

    lv_meter_indicator_t *indic;

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(heel_display, scale, 7, lv_palette_main(LV_PALETTE_ORANGE), -10);

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_heel_value);
    lv_anim_set_var(&a, indic);
    lv_anim_set_values(&a, 5, 15);
    lv_anim_set_time(&a, 5000);
    lv_anim_set_repeat_delay(&a, 3000);
    lv_anim_set_playback_time(&a, 5000);
    lv_anim_set_playback_delay(&a, 3000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
  }

  static void heel_update_cb() {
    // TODO:
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

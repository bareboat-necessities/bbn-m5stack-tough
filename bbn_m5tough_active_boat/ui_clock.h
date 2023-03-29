#ifndef UI_CLOCK_H
#define UI_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t clockScreen;

  /**
   * A clock display 
   */
  void lv_clock_display(lv_obj_t *parent) {
    clock_display = lv_meter_create(parent);
    lv_obj_set_size(clock_display, 220, 220);
    lv_obj_center(clock_display);

    /*Create a scale for the minutes*/
    /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
    lv_meter_scale_t *scale = lv_meter_add_scale(clock_display);
    lv_meter_set_scale_ticks(clock_display, scale, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(clock_display, scale, 0, 60, 360, 270);

    /*Create another scale for the hours. It's only visual and contains only major ticks*/
    lv_meter_scale_t *scale_hour = lv_meter_add_scale(clock_display);
    lv_meter_set_scale_ticks(clock_display, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));           /*12 ticks*/
    lv_meter_set_scale_major_ticks(clock_display, scale_hour, 1, 2, 20, lv_palette_main(LV_PALETTE_GREY), 10); /*Every tick is major*/
    lv_meter_set_scale_range(clock_display, scale_hour, 1, 12, 330, 300);                                      /*[1..12] values in an almost full circle*/

    indic_sec = lv_meter_add_needle_line(clock_display, scale, 2, lv_palette_main(LV_PALETTE_GREY), -10);
    indic_min = lv_meter_add_needle_line(clock_display, scale, 4, lv_palette_main(LV_PALETTE_GREEN), -20);
    indic_hour = lv_meter_add_needle_line(clock_display, scale, 7, lv_palette_main(LV_PALETTE_RED), -42);
  }

  void init_clockScreen() {
    clockScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_clock_display(clockScreen.screen);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

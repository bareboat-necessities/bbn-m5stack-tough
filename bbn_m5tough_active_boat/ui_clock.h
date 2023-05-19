#ifndef UI_CLOCK_H
#define UI_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t clockScreen;

  static lv_obj_t *clock_display;

  lv_meter_indicator_t *indic_sec;
  lv_meter_indicator_t *indic_min;
  lv_meter_indicator_t *indic_hour;

  lv_obj_t *labelDate;
  lv_obj_t *labelTime;

  /**
   * A clock display 
   */
  static void lv_clock_display(lv_obj_t *parent) {
    clock_display = lv_meter_create(parent);
    lv_obj_set_style_pad_all(clock_display, 5, LV_PART_MAIN);

    lv_obj_set_size(clock_display, 200, 200);
    lv_obj_center(clock_display);

    labelDate = lv_label_create(parent);
    lv_obj_set_pos(labelDate, 10, 10);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(labelDate, &lv_font_montserrat_20, 0);
#endif
    lv_obj_align(labelDate, LV_ALIGN_TOP_LEFT, 2, 2);

    labelTime = lv_label_create(parent);
    lv_obj_set_pos(labelTime, 250, 10);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(labelTime, &lv_font_montserrat_20, 0);
#endif
    lv_obj_align(labelTime, LV_ALIGN_TOP_LEFT, 230, 2);

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

  static void set_clock_value(void *indic, int32_t v) {
    lv_meter_set_indicator_value(clock_display, (lv_meter_indicator_t *)indic, v);
  }

  static void clock_update_cb() {
    M5.Rtc.GetDate(&RTCdate);
    M5.Rtc.GetTime(&RTCtime);
    set_clock_value(indic_hour, ((RTCtime.Hours % 12) * 60 + RTCtime.Minutes) / 12);
    set_clock_value(indic_min, RTCtime.Minutes);
    set_clock_value(indic_sec, RTCtime.Seconds);

    char bufferDate[14];
    char bufferTime[12];

    sprintf(bufferDate, "%04d-%02d-%02d", 100 + RTCdate.Year, 1 + RTCdate.Month, RTCdate.Date);
    sprintf(bufferTime, "%02d:%02d:%02d", RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);

    lv_label_set_text(labelDate, bufferDate);
    lv_label_set_text(labelTime, bufferTime);
  }

  void init_clockScreen() {
    clockScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    clockScreen.init_cb = lv_clock_display;
    clockScreen.update_cb = clock_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

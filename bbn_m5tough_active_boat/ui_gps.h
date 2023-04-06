#ifndef UI_GPS_H
#define UI_GPS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t gpsScreen;

  static lv_obj_t *gps_time_label;
  static lv_obj_t *gps_cog_label;
  static lv_obj_t *gps_lat_label;
  static lv_obj_t *gps_lon_label;

  /**
   * A GPS display 
   */
  void lv_gps_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -110);
    lv_label_set_text(main_label, "GPS");

    gps_time_label = lv_label_create(parent);
    lv_obj_align(gps_time_label, LV_ALIGN_TOP_LEFT, 10, 40);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(gps_time_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(gps_time_label, "GPS TIME:        --");

    gps_lat_label = lv_label_create(parent);
    lv_obj_align(gps_lat_label, LV_ALIGN_TOP_LEFT, 10, 80);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(gps_lat_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(gps_lat_label, "LAT:                    --");

    gps_lon_label = lv_label_create(parent);
    lv_obj_align(gps_lon_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(gps_lon_label, &lv_font_montserrat_20, NULL);
#endif
    lv_label_set_text(gps_lon_label, "LON:                   --");
  }

  static void gps_update_cb() {
  }

  void init_gpsScreen() {
    gpsScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_gps_display(gpsScreen.screen);
    gpsScreen.update_cb = gps_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

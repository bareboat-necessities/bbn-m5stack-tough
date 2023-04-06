#ifndef UI_GPS_H
#define UI_GPS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t gpsScreen;

  static lv_obj_t *cog_label;
  static lv_obj_t *lat_label;
  static lv_obj_t *lon_label;

  /**
   * A GPS display 
   */
  void lv_gps_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text(main_label, "GPS");
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


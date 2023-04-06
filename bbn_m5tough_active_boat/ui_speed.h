#ifndef UI_SPEED_H
#define UI_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t speedScreen;

  static lv_obj_t *sog_label;

  /**
   * A speed display 
   */
  void lv_speed_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text(main_label, "SPEED");
  }

  static void speed_update_cb() {
  }

  void init_speedScreen() {
    speedScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_speed_display(speedScreen.screen);
    speedScreen.update_cb = speed_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

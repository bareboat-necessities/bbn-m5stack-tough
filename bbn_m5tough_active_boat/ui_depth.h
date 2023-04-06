#ifndef UI_DEPTH_H
#define UI_DEPTH_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t depthScreen;

  static lv_obj_t *dbt_label;
  static lv_obj_t *dbk_label;

  /**
   * A depth display 
   */
  void lv_depth_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -60);
    lv_label_set_text(main_label, "DEPTH");
  }

  static void depth_update_cb() {
  }

  void init_depthScreen() {
    depthScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_depth_display(depthScreen.screen);
    depthScreen.update_cb = depth_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

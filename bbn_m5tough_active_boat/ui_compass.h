#ifndef UI_COMPASS_H
#define UI_COMPASS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t compassScreen;

  static lv_obj_t *compass_img;

  LV_IMG_DECLARE(ui_img_compass_png);

  /**
   * A compass display (clinometer)
   */
  void lv_compass_display(lv_obj_t *parent) {
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN);
    compass_img = lv_img_create(parent);
    lv_img_set_src(compass_img, &ui_img_compass_png);
    lv_obj_align(compass_img, LV_ALIGN_CENTER, 0, 0);
  }

  static void compass_update_cb() {
    lv_img_set_angle(compass_img, 450);
  }

  void init_compassScreen() {
    compassScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_compass_display(compassScreen.screen);
    compassScreen.update_cb = compass_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

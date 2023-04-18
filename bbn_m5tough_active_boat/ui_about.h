#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t aboutScreen;

  LV_IMG_DECLARE(bareboat_necessities_logo);

  static void about_display(lv_obj_t *parent) {
    lv_obj_t *img = lv_img_create(parent);
    lv_obj_set_style_bg_color(parent, lv_color_white(), LV_PART_MAIN);
    lv_img_set_src(img, &bareboat_necessities_logo);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
  }
  
  void init_aboutScreen() {
    aboutScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    aboutScreen.init_cb = about_display;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

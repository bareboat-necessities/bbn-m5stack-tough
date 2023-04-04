#ifndef UI_COMPASS_H
#define UI_COMPASS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t compassScreen;

  static lv_obj_t *compass_img;
  static lv_obj_t *compass_l;

  LV_IMG_DECLARE(ui_img_compass_png);

  /**
   * A compass display (clinometer)
   */
  void lv_compass_display(lv_obj_t *parent) {
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN);
    compass_img = lv_img_create(parent);
    lv_img_set_src(compass_img, &ui_img_compass_png);
    lv_obj_align(compass_img, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *compass_mark_l = lv_label_create(parent);
    lv_label_set_text(compass_mark_l, LV_SYMBOL_DOWN);
    lv_obj_align(compass_mark_l, LV_ALIGN_CENTER, 0, -100);

    compass_l = lv_label_create(parent);
    lv_label_set_text(compass_l, "--" LV_SYMBOL_DEGREES);
    //lv_obj_set_width(compass_l, width_l);
    //lv_obj_set_style_text_align(compass_l, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(compass_l, LV_ALIGN_CENTER, 0, 0);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(compass_l, &lv_font_montserrat_20, NULL);
#endif
  }

  static void compass_update_cb() {
    int16_t h_deg = fresh(shipDataModel.navigation.heading_mag.age) ? shipDataModel.navigation.heading_mag.deg : 0;
    lv_img_set_angle(compass_img, h_deg * 10);
    lv_label_set_text(compass_l, 
                      (fresh(shipDataModel.navigation.heading_mag.age) ? String(shipDataModel.navigation.heading_mag.deg, 0) + LV_SYMBOL_DEGREES : "--").c_str());
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

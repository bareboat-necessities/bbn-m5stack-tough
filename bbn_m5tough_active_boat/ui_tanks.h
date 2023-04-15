#ifndef UI_TANKS_H
#define UI_TANKS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t tanksScreen;

  static lv_obj_t *bar_w1;
  static lv_obj_t *bar_w2;
  static lv_obj_t *bar_bw1;
  static lv_obj_t *bar_bw2;
  static lv_obj_t *bar_f1;
  static lv_obj_t *bar_f2;
  static lv_obj_t *bar_g1;
  static lv_obj_t *bar_g2;

  /**
 * A tanks display 
 */
  void lv_tanks_display(lv_obj_t *parent) {
    bar_w1 = lv_bar_create(parent);
    lv_obj_set_size(bar_w1, 50, 70);
    lv_obj_set_style_bg_color(bar_w1, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_radius(bar_w1, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_w1, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_w1, LV_ALIGN_CENTER, -120, -40);
    lv_bar_set_range(bar_w1, 0, 100);

    lv_obj_t *bar_w1_l = lv_label_create(parent);
    lv_obj_align(bar_w1_l, LV_ALIGN_CENTER, -120, -40 - 60);
    lv_label_set_text(bar_w1_l, "Fresh 1\n--%");

    bar_w2 = lv_bar_create(parent);
    lv_obj_set_size(bar_w2, 50, 70);
    lv_obj_set_style_bg_color(bar_w2, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_radius(bar_w2, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_w2, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_w2, LV_ALIGN_CENTER, -40, -40);
    lv_bar_set_range(bar_w2, 0, 100);

    lv_obj_t *bar_w2_l = lv_label_create(parent);
    lv_obj_align(bar_w2_l, LV_ALIGN_CENTER, -40, -40 - 60);
    lv_label_set_text(bar_w2_l, "Fresh 2\n--%");

    bar_bw1 = lv_bar_create(parent);
    lv_obj_set_size(bar_bw1, 50, 70);
    lv_obj_set_style_bg_color(bar_bw1, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_bw1, lv_palette_lighten(LV_PALETTE_RED, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_bw1, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_bw1, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_bw1, LV_ALIGN_CENTER, 40, -40);
    lv_bar_set_range(bar_bw1, 0, 100);

    lv_obj_t *bar_bw1_l = lv_label_create(parent);
    lv_obj_align(bar_bw1_l, LV_ALIGN_CENTER, 40, -40 - 60);
    lv_label_set_text(bar_bw1_l, "Black 1\n--%");

    bar_bw2 = lv_bar_create(parent);
    lv_obj_set_size(bar_bw2, 50, 70);
    lv_obj_set_style_bg_color(bar_bw2, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_bw2, lv_palette_lighten(LV_PALETTE_RED, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_bw2, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_bw2, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_bw2, LV_ALIGN_CENTER, 120, -40);
    lv_bar_set_range(bar_bw2, 0, 100);

    lv_obj_t *bar_bw2_l = lv_label_create(parent);
    lv_obj_align(bar_bw2_l, LV_ALIGN_CENTER, 120, -40 - 60);
    lv_label_set_text(bar_bw2_l, "Black 2\n--%");

    bar_f1 = lv_bar_create(parent);
    lv_obj_set_size(bar_f1, 50, 70);
    lv_obj_set_style_bg_color(bar_f1, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_f1, lv_palette_lighten(LV_PALETTE_GREEN, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_f1, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_f1, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_f1, LV_ALIGN_CENTER, -120, 40);
    lv_bar_set_range(bar_f1, 0, 100);

    lv_obj_t *bar_f1_l = lv_label_create(parent);
    lv_obj_align(bar_f1_l, LV_ALIGN_CENTER, -120, 40 + 60);
    lv_label_set_text(bar_f1_l, "Fuel 1\n--%");

    bar_f2 = lv_bar_create(parent);
    lv_obj_set_size(bar_f2, 50, 70);
    lv_obj_set_style_bg_color(bar_f2, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_f2, lv_palette_lighten(LV_PALETTE_GREEN, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_f2, 6, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_f2, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_f2, LV_ALIGN_CENTER, -40, 40);
    lv_bar_set_range(bar_f2, 0, 100);

    lv_obj_t *bar_f2_l = lv_label_create(parent);
    lv_obj_align(bar_f2_l, LV_ALIGN_CENTER, -40, 40 + 60);
    lv_label_set_text(bar_f2_l, "Fuel 2\n--%");

    bar_g1 = lv_bar_create(parent);
    lv_obj_set_size(bar_g1, 50, 70);
    lv_obj_set_style_bg_color(bar_g1, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_g1, lv_palette_lighten(LV_PALETTE_ORANGE, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_g1, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_g1, LV_ALIGN_CENTER, 40, 40);
    lv_bar_set_range(bar_g1, 0, 100);

    lv_obj_t *bar_g1_l = lv_label_create(parent);
    lv_obj_align(bar_g1_l, LV_ALIGN_CENTER, 40, 40 + 60);
    lv_label_set_text(bar_g1_l, "Gas 1\n--%");

    bar_g2 = lv_bar_create(parent);
    lv_obj_set_size(bar_g2, 50, 70);
    lv_obj_set_style_bg_color(bar_g2, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_g2, lv_palette_lighten(LV_PALETTE_ORANGE, 1), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar_g2, 0, LV_PART_INDICATOR);
    lv_obj_align(bar_g2, LV_ALIGN_CENTER, 120, 40);
    lv_bar_set_range(bar_g2, 0, 100);

    lv_obj_t *bar_g2_l = lv_label_create(parent);
    lv_obj_align(bar_g2_l, LV_ALIGN_CENTER, 120, 40 + 60);
    lv_label_set_text(bar_g2_l, "Gas 2\n--%");
  }

  static void tanks_update_cb() {
    lv_bar_set_value(bar_w1, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_w2, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_bw1, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_bw2, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_f1, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_f2, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_g1, 50, LV_ANIM_OFF);
    lv_bar_set_value(bar_g2, 50, LV_ANIM_OFF);
  }

  void init_tanksScreen() {
    tanksScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_tanks_display(tanksScreen.screen);
    tanksScreen.update_cb = tanks_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

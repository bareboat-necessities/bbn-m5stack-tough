#ifndef UI_COMPASS_H
#define UI_COMPASS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t compassScreen;

  static lv_obj_t *compass_display;
  static lv_obj_t *compass_l;
  static lv_obj_t *compass_hdt_l;
  static lv_obj_t *compass_cogt_l;
  static lv_obj_t *compass_mag_var_l;

  static lv_obj_t *labelScont;
  static lv_obj_t *labelNcont;
  static lv_obj_t *labelEcont;
  static lv_obj_t *labelWcont;

  static lv_meter_scale_t *scale_compass;
  static lv_meter_scale_t *scale_compass_maj;

  /**
   * A compass display
   */
  static void lv_compass_display(lv_obj_t *parent) {
    compass_display = lv_meter_create(parent);

    lv_obj_remove_style(compass_display, NULL, LV_PART_MAIN);
    lv_obj_remove_style(compass_display, NULL, LV_PART_INDICATOR);

    lv_obj_set_size(compass_display, 190, 190);
    lv_obj_center(compass_display);

    scale_compass = lv_meter_add_scale(compass_display);
    lv_meter_set_scale_ticks(compass_display, scale_compass, 73, 1, 12, lv_palette_main(LV_PALETTE_GREY));

    scale_compass_maj = lv_meter_add_scale(compass_display);
    lv_meter_set_scale_ticks(compass_display, scale_compass_maj, 12, 2, 15, lv_palette_main(LV_PALETTE_GREY)); /*12 ticks*/

    labelNcont = lv_obj_create(parent);
    lv_obj_set_size(labelNcont, 42, 42);
    lv_obj_set_style_pad_all(labelNcont, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(labelNcont, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
    lv_obj_align(labelNcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelN = lv_label_create(labelNcont);
    lv_label_set_text_static(labelN, "N");
    lv_obj_align(labelN, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelNcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelNcont, 42 / 2 + 48, 0);

    labelScont = lv_obj_create(parent);
    lv_obj_set_size(labelScont, 42, 42);
    lv_obj_set_style_pad_all(labelScont, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(labelScont, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_align(labelScont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelS = lv_label_create(labelScont);
    lv_label_set_text_static(labelS, "S");
    lv_obj_align(labelS, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelScont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelScont, 42 / 2 + 48, 0);

    labelEcont = lv_obj_create(parent);
    lv_obj_set_size(labelEcont, 42, 42);
    lv_obj_set_style_pad_all(labelEcont, 2, LV_PART_MAIN);
    lv_obj_align(labelEcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelE = lv_label_create(labelEcont);
    lv_label_set_text_static(labelE, "E");
    lv_obj_align(labelE, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelEcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelEcont, 42 / 2 + 48, 0);

    labelWcont = lv_obj_create(parent);
    lv_obj_set_size(labelWcont, 42, 42);
    lv_obj_set_style_pad_all(labelWcont, 2, LV_PART_MAIN);
    lv_obj_align(labelWcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelW = lv_label_create(labelWcont);
    lv_label_set_text_static(labelW, "W");
    lv_obj_align(labelW, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelWcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelWcont, 42 / 2 + 48, 0);

    lv_obj_t *compass_mark_l = lv_label_create(parent);
    lv_label_set_text_static(compass_mark_l, LV_SYMBOL_DOWN);
    lv_obj_align(compass_mark_l, LV_ALIGN_CENTER, 0, -100);

    compass_l = lv_label_create(parent);
    lv_label_set_text_static(compass_l, "--" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_l, LV_ALIGN_CENTER, 0, 0);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(compass_l, &lv_font_montserrat_20, 0);
#endif

    lv_meter_set_scale_range(compass_display, scale_compass, 0, 72, 360, 270);
    lv_meter_set_scale_range(compass_display, scale_compass_maj, 1, 12, 330, 300);
    lv_obj_set_style_transform_angle(labelScont, 180 * 10, 0);
    lv_obj_set_style_transform_angle(labelEcont, 90 * 10, 0);
    lv_obj_set_style_transform_angle(labelWcont, 270 * 10, 0);

    compass_hdt_l = lv_label_create(parent);
    lv_label_set_text_static(compass_hdt_l, "HDT:  --" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_hdt_l, LV_ALIGN_TOP_LEFT, 2, 2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(compass_hdt_l, &lv_font_montserrat_20, 0);
#endif

    compass_cogt_l = lv_label_create(parent);
    lv_label_set_text_static(compass_cogt_l, "COGT:   --" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_cogt_l, LV_ALIGN_TOP_RIGHT, -2, 2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(compass_cogt_l, &lv_font_montserrat_20, 0);
#endif

    compass_mag_var_l = lv_label_create(parent);
    lv_label_set_text_static(compass_mag_var_l, "Var:\n--" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_mag_var_l, LV_ALIGN_BOTTOM_LEFT, 2, -2);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(compass_mag_var_l, &lv_font_montserrat_20, 0);
#endif
  }

  static int16_t last_heading = 0;
  static unsigned long last_compass_upd = 0;

  static void compass_update_cb() {
    if (millis() - last_compass_upd > 500) {  // reduce expensive rotations
      int16_t h_deg = fresh(shipDataModel.navigation.heading_mag.age) ? shipDataModel.navigation.heading_mag.deg : 0;
      if (last_heading != h_deg) {
        int rot = 360 - h_deg;
        lv_meter_set_scale_range(compass_display, scale_compass, 0, 72, 360, 270 + rot);
        lv_meter_set_scale_range(compass_display, scale_compass_maj, 1, 12, 330, 300 + rot);
        lv_obj_set_style_transform_angle(labelNcont, rot * 10, 0);
        lv_obj_set_style_transform_angle(labelScont, (180 + rot) * 10, 0);
        lv_obj_set_style_transform_angle(labelEcont, (90 + rot) * 10, 0);
        lv_obj_set_style_transform_angle(labelWcont, (270 + rot) * 10, 0);

        lv_label_set_text(compass_l,
                          (fresh(shipDataModel.navigation.heading_mag.age) ? String(shipDataModel.navigation.heading_mag.deg, 0) += LV_SYMBOL_DEGREES : "--").c_str());
        lv_label_set_text(compass_mag_var_l,
                          (fresh(shipDataModel.navigation.mag_var.age, LONG_EXPIRE_TO) ? String("Var:\n") += String(shipDataModel.navigation.mag_var.deg, 1) += LV_SYMBOL_DEGREES : ("Var:\n--" LV_SYMBOL_DEGREES)).c_str());
        lv_label_set_text(compass_hdt_l,
                          (String("HDT:  ") += (fresh(shipDataModel.navigation.heading_true.age) ? String(shipDataModel.navigation.heading_true.deg, 0) += LV_SYMBOL_DEGREES : "--"))
                            .c_str());
        lv_label_set_text(compass_cogt_l,
                          (String("COGT:  ") += (fresh(shipDataModel.navigation.course_over_ground_true.age) ? String(shipDataModel.navigation.course_over_ground_true.deg, 0) += String(LV_SYMBOL_DEGREES) : "--"))
                            .c_str());
      }
      last_compass_upd = millis();
    }
  }

  void init_compassScreen() {
    compassScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    compassScreen.init_cb = lv_compass_display;
    compassScreen.update_cb = compass_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

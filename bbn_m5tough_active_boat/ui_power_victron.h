#ifndef UI_POWER_VICTRON_H
#define UI_POWER_VICTRON_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t victronScreen;

  /**
   * A victron display 
   */
  void lv_victron_display(lv_obj_t *parent) {
    static lv_style_t style_shadow;
    lv_style_init(&style_shadow);

    /*Set a background color and a radius*/
    lv_style_set_radius(&style_shadow, 3);
    lv_style_set_shadow_width(&style_shadow, 3);
    lv_style_set_pad_all(&style_shadow, 7);
    lv_style_set_bg_opa(&style_shadow, LV_OPA_COVER);
    lv_style_set_bg_color(&style_shadow, lv_palette_lighten(LV_PALETTE_GREY, 1));

    /*Add border to the bottom+right*/
    lv_style_set_border_color(&style_shadow, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_shadow, 1);
    lv_style_set_border_opa(&style_shadow, LV_OPA_50);
    lv_style_set_border_side(&style_shadow, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

    lv_obj_t *shore;
    shore = lv_obj_create(parent);
    lv_obj_add_style(shore, &style_shadow, 0);
    lv_obj_set_size(shore, 94, 110);
    lv_obj_align(shore, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_bg_color(shore, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    lv_obj_t *shore_label = lv_label_create(shore);
    lv_label_set_text(shore_label, "Shore\n1205 W");
    lv_obj_align(shore_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *inverters;
    inverters = lv_obj_create(parent);
    lv_obj_add_style(inverters, &style_shadow, 0);
    lv_obj_set_size(inverters, 94, 80);
    lv_obj_align(inverters, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_color(inverters, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);

    lv_obj_t *inv_label = lv_label_create(inverters);
    lv_label_set_text(inv_label, "Inverters");
    lv_obj_align(inv_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *ac;
    ac = lv_obj_create(parent);
    lv_obj_add_style(ac, &style_shadow, 0);
    lv_obj_set_size(ac, 94, 110);
    lv_obj_align(ac, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_set_style_bg_color(ac, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);

    lv_obj_t *ac_label = lv_label_create(ac);
    lv_label_set_text(ac_label, "AC Load\n340W");
    lv_obj_align(ac_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *dc;
    dc = lv_obj_create(parent);
    lv_obj_add_style(dc, &style_shadow, 0);
    lv_obj_set_size(dc, 94, 110);
    lv_obj_align(dc, LV_ALIGN_BOTTOM_LEFT, 5, -5);
    lv_obj_set_style_bg_color(dc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);

    lv_obj_t *dc_label = lv_label_create(dc);
    lv_label_set_text(dc_label, "Batteries\n87%");
    lv_obj_align(dc_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *dc_ld;
    dc_ld = lv_obj_create(parent);
    lv_obj_add_style(dc_ld, &style_shadow, 0);
    lv_obj_set_size(dc_ld, 94, 60);
    lv_obj_align(dc_ld, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_bg_color(dc_ld, lv_palette_main(LV_PALETTE_TEAL), LV_PART_MAIN);

    lv_obj_t *dc_ld_label = lv_label_create(dc_ld);
    lv_label_set_text(dc_ld_label, "DC Load\n150 W");
    lv_obj_align(dc_ld_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *pv;
    pv = lv_obj_create(parent);
    lv_obj_add_style(pv, &style_shadow, 0);
    lv_obj_set_size(pv, 94, 110);
    lv_obj_align(pv, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
    lv_obj_set_style_bg_color(pv, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);

    lv_obj_t *pv_label = lv_label_create(pv);
    lv_label_set_text(pv_label, "PV\n247 W");
    lv_obj_align(pv_label, LV_ALIGN_TOP_LEFT, 0, 0);

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    static lv_point_t line_pt1[] = { { 98, 60 }, { 112, 60 } };
    lv_obj_t *line1 = lv_line_create(parent);
    lv_obj_align(line1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(line1, &style_line, 0);
    lv_line_set_points(line1, line_pt1, 2); /*Set the points*/

    static lv_point_t line_pt2[] = { { 208, 60 }, { 222, 60 } };
    lv_obj_t *line2 = lv_line_create(parent);
    lv_obj_align(line2, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(line2, &style_line, 0);
    lv_line_set_points(line2, line_pt2, 2); /*Set the points*/

    static lv_point_t line_pt3[] = { { 98, 203 }, { 112, 203 } };
    lv_obj_t *line3 = lv_line_create(parent);
    lv_obj_align(line3, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(line3, &style_line, 0);
    lv_line_set_points(line3, line_pt3, 2); /*Set the points*/

    static lv_point_t line_pt4[] = { { 98, 150 }, { 222, 150 } };
    lv_obj_t *line4 = lv_line_create(parent);
    lv_obj_align(line4, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(line4, &style_line, 0);
    lv_line_set_points(line4, line_pt4, 2); /*Set the points*/

    static lv_point_t line_pt5[] = { { 160, 102 }, { 160, 150 } };
    lv_obj_t *line5 = lv_line_create(parent);
    lv_obj_align(line5, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(line5, &style_line, 0);
    lv_line_set_points(line5, line_pt5, 2); /*Set the points*/
  }

  static void victron_update_cb() {
    // TODO:
  }

  void init_victronScreen() {
    victronScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_victron_display(victronScreen.screen);
    victronScreen.update_cb = victron_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

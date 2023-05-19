#ifndef UI_TANKS_H
#define UI_TANKS_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t tanksScreen;

  static lv_obj_t *bar_tank[MAX_TANKS];
  static lv_obj_t *bar_tank_l[MAX_TANKS];

  /**
   * A tanks display 
   */
  static void lv_tanks_display(lv_obj_t *parent) {
    for (int i = 0; i < MAX_TANKS; i++) {
      bar_tank[i] = lv_bar_create(parent);
      lv_obj_set_size(bar_tank[i], 50, 70);
      lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
      lv_obj_set_style_radius(bar_tank[i], 0, LV_PART_INDICATOR);
      lv_bar_set_range(bar_tank[i], 0, 100);
      bar_tank_l[i] = lv_label_create(parent);
      lv_label_set_text_static(bar_tank_l[i], "--");
    }

    lv_obj_align(bar_tank[0], LV_ALIGN_CENTER, -120, -40);
    lv_obj_align(bar_tank[1], LV_ALIGN_CENTER, -40, -40);
    lv_obj_align(bar_tank[2], LV_ALIGN_CENTER, 40, -40);
    lv_obj_align(bar_tank[3], LV_ALIGN_CENTER, 120, -40);
    lv_obj_align(bar_tank[4], LV_ALIGN_CENTER, -120, 40);
    lv_obj_align(bar_tank[5], LV_ALIGN_CENTER, -40, 40);
    lv_obj_align(bar_tank[6], LV_ALIGN_CENTER, 40, 40);
    lv_obj_align(bar_tank[7], LV_ALIGN_CENTER, 120, 40);

    lv_obj_align(bar_tank_l[0], LV_ALIGN_CENTER, -120, -40 - 60);
    lv_obj_align(bar_tank_l[1], LV_ALIGN_CENTER, -40, -40 - 60);
    lv_obj_align(bar_tank_l[2], LV_ALIGN_CENTER, 40, -40 - 60);
    lv_obj_align(bar_tank_l[3], LV_ALIGN_CENTER, 120, -40 - 60);
    lv_obj_align(bar_tank_l[4], LV_ALIGN_CENTER, -120, 40 + 60);
    lv_obj_align(bar_tank_l[5], LV_ALIGN_CENTER, -40, 40 + 60);
    lv_obj_align(bar_tank_l[6], LV_ALIGN_CENTER, 40, 40 + 60);
    lv_obj_align(bar_tank_l[7], LV_ALIGN_CENTER, 120, 40 + 60);
  }

  static void tanks_update_cb() {
    for (int i = 0; i < MAX_TANKS; i++) {
      float pct = shipDataModel.tanks.tank[i].percent_of_full.pct;
      if (fresh(shipDataModel.tanks.tank[i].percent_of_full.age, 7200000 /*2 hour*/)) {
        lv_bar_set_value(bar_tank[i], pct, LV_ANIM_OFF);
        if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::FUEL) {
          lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_GREEN, 1), LV_PART_INDICATOR);
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text(bar_tank_l[i], (String("Fuel\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::BLACK_WATER) {
          lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_RED, 1), LV_PART_INDICATOR);
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text(bar_tank_l[i], (String("Black\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::WASTE_WATER) {
          lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_RED, 1), LV_PART_INDICATOR);
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text(bar_tank_l[i], (String("Grey\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::FRESH_WATER) {
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text(bar_tank_l[i], (String("Fresh\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::LUBRICATION) {
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_ORANGE, 1), LV_PART_INDICATOR);
          lv_label_set_text(bar_tank_l[i], (String("Lube\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::LIVE_WELL) {
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text(bar_tank_l[i], (String("Live Well\n") += String(pct, 1) += "%").c_str());
        } else if (shipDataModel.tanks.tank[i].fluid_type == fluid_type_e::GAS) {
          lv_obj_set_style_bg_color(bar_tank[i], lv_palette_lighten(LV_PALETTE_ORANGE, 1), LV_PART_INDICATOR);
          lv_label_set_text(bar_tank_l[i], (String("Gas\n") += String(pct, 1) += "%").c_str());
        } else {
          lv_obj_set_style_radius(bar_tank[i], 6, LV_PART_MAIN);
          lv_label_set_text_static(bar_tank_l[i], "--");
        }
      }
    }
  }

  void init_tanksScreen() {
    tanksScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    tanksScreen.init_cb = lv_tanks_display;
    tanksScreen.update_cb = tanks_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef UI_TRIP_DATA_H
#define UI_TRIP_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t tripDataScreen;

  static lv_obj_t *xte_label;
  static lv_obj_t *brgt_label;
  static lv_obj_t *brgm_label;
  static lv_obj_t *hdm_leg_label;
  static lv_obj_t *vmg_label;
  static lv_obj_t *rng_label;
  static lv_obj_t *ttg_at_vmg_label;

  /**
   * Trip data display 
   */
  static void lv_trip_data_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_recolor(main_label, true);
    lv_label_set_text_static(main_label, "TRIP LEG  #0080ff " LV_SYMBOL_GPS " #");

    xte_label = lv_label_create(parent);
    lv_obj_align(xte_label, LV_ALIGN_TOP_LEFT, 10, 30);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(xte_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(xte_label, "XTE (nm):                 --");

    brgt_label = lv_label_create(parent);
    lv_obj_align(brgt_label, LV_ALIGN_TOP_LEFT, 10, 60);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(brgt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(brgt_label, "BRG (t deg):            --");

    brgm_label = lv_label_create(parent);
    lv_obj_align(brgm_label, LV_ALIGN_TOP_LEFT, 10, 90);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(brgm_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(brgm_label, "BRG (m deg):         --");

    hdm_leg_label = lv_label_create(parent);
    lv_obj_align(hdm_leg_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(hdm_leg_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(hdm_leg_label, "HDM (deg):             --");

    vmg_label = lv_label_create(parent);
    lv_obj_align(vmg_label, LV_ALIGN_TOP_LEFT, 10, 150);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vmg_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vmg_label, "VMG (kn):                 --");

    rng_label = lv_label_create(parent);
    lv_obj_align(rng_label, LV_ALIGN_TOP_LEFT, 10, 180);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(rng_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(rng_label, "RNG (nm):                --");

    ttg_at_vmg_label = lv_label_create(parent);
    lv_obj_align(ttg_at_vmg_label, LV_ALIGN_TOP_LEFT, 10, 210);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(ttg_at_vmg_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(ttg_at_vmg_label, "TTG@VMG (hr):      --");
  }

  static void trip_data_update_cb() {
    lv_label_set_text(xte_label,
                      (String("XTE (nm):                 ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.cross_track_error.age, 30000)
                            ? String(shipDataModel.navigation.course_rhumbline.cross_track_error.m / NM_TO_METERS, 3)
                            : String("--")))
                        .c_str());
    lv_label_set_text(brgt_label,
                      (String("BRG (t deg):            ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.bearing_track_true.age)
                            ? String(shipDataModel.navigation.course_rhumbline.bearing_track_true.deg, 0) += LV_SYMBOL_DEGREES
                            : String("--" LV_SYMBOL_DEGREES)))
                        .c_str());
    lv_label_set_text(brgm_label,
                      (String("BRG (m deg):         ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.bearing_track_mag.age)
                            ? String(shipDataModel.navigation.course_rhumbline.bearing_track_mag.deg, 0) += LV_SYMBOL_DEGREES
                            : String("--" LV_SYMBOL_DEGREES)))
                        .c_str());
    lv_label_set_text(hdm_leg_label,
                      (String("HDM (deg):             ")
                       += (fresh(shipDataModel.navigation.heading_mag.age)
                            ? String(shipDataModel.navigation.heading_mag.deg, 0) += LV_SYMBOL_DEGREES
                            : String("--" LV_SYMBOL_DEGREES)))
                        .c_str());
    lv_label_set_text(vmg_label,
                      (String("VMG (kn):                 ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.age, 10000)
                            ? String(shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.kn, 2)
                            : String("--")))
                        .c_str());
    lv_label_set_text(rng_label,
                      (String("RNG (nm):                ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.next_point.distance.age, 10000)
                            ? String(shipDataModel.navigation.course_rhumbline.next_point.distance.m / NM_TO_METERS, 1)
                            : String("--")))
                        .c_str());
    lv_label_set_text(ttg_at_vmg_label,
                      (String("TTG@VMG (hr):      ")
                       += (fresh(shipDataModel.navigation.course_rhumbline.next_point.distance.age, 10000)
                              && fresh(shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.age, 10000)
                              && shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.kn > 0.001
                            ? String(shipDataModel.navigation.course_rhumbline.next_point.distance.m / NM_TO_METERS
                                       / shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.kn,
                                     2)
                            : String("--")))
                        .c_str());
  }

  void init_tripDataScreen() {
    tripDataScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    tripDataScreen.init_cb = lv_trip_data_display;
    tripDataScreen.update_cb = trip_data_update_cb;
  }


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

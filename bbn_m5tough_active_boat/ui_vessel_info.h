#ifndef UI_VESSEL_INFO_H
#define UI_VESSEL_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t vesselScreen;

  static lv_obj_t *vessel_name_label;
  static lv_obj_t *vessel_mmsi_label;
  static lv_obj_t *vessel_length_label;
  static lv_obj_t *vessel_draft_label;
  static lv_obj_t *vessel_beam_label;
  static lv_obj_t *vessel_air_height_label;
  static lv_obj_t *vessel_nav_status_label;

  /**
   * A vessel display 
   */
  static void lv_vessel_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_recolor(main_label, true);
    lv_label_set_text(main_label, "VESSEL  #0080ff " LV_SYMBOL_LIST " #");

    vessel_name_label = lv_label_create(parent);
    lv_obj_align(vessel_name_label, LV_ALIGN_TOP_LEFT, 10, 30);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_name_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_name_label, "Name:           --");

    vessel_mmsi_label = lv_label_create(parent);
    lv_obj_align(vessel_mmsi_label, LV_ALIGN_TOP_LEFT, 10, 60);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_mmsi_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_mmsi_label, "MMSI:          --");

    vessel_length_label = lv_label_create(parent);
    lv_obj_align(vessel_length_label, LV_ALIGN_TOP_LEFT, 10, 90);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_length_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_length_label, "Length:          --");

    vessel_draft_label = lv_label_create(parent);
    lv_obj_align(vessel_draft_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_draft_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_draft_label, "Draft:          --");

    vessel_beam_label = lv_label_create(parent);
    lv_obj_align(vessel_beam_label, LV_ALIGN_TOP_LEFT, 10, 150);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_beam_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_beam_label, "Beam:          --");

    vessel_air_height_label = lv_label_create(parent);
    lv_obj_align(vessel_air_height_label, LV_ALIGN_TOP_LEFT, 10, 180);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_air_height_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_air_height_label, "Air height:   --");

    vessel_nav_status_label = lv_label_create(parent);
    lv_obj_align(vessel_nav_status_label, LV_ALIGN_TOP_LEFT, 10, 210);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_nav_status_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(vessel_nav_status_label, "Nav Status:     --");
  }

  static void vessel_update_cb() {
    lv_label_set_text(vessel_name_label,
                      (String(F("Name:         ")) 
                      + (isSet(shipDataModel.vessel.name) ? String(shipDataModel.vessel.name) : String("--"))).c_str());
    lv_label_set_text(vessel_mmsi_label,
                      (String(F("MMSI:         ")) 
                      + (isSet(shipDataModel.vessel.mmsi) ? String(shipDataModel.vessel.mmsi) : String("--"))).c_str());
    lv_label_set_text(vessel_length_label,
                      (String(F("Length:         ")) 
                      + (shipDataModel.design.length.m > 0 ? String(shipDataModel.design.length.m * _GPS_FEET_PER_METER, 1) + "'" : String("--"))).c_str());
    lv_label_set_text(vessel_draft_label,
                      (String(F("Draft:         ")) 
                      + (shipDataModel.design.draft.m > 0 ? String(shipDataModel.design.draft.m * _GPS_FEET_PER_METER, 1) + "'" : String("--"))).c_str());
    lv_label_set_text(vessel_beam_label,
                      (String(F("Beam:         ")) 
                      + (shipDataModel.design.beam.m > 0 ? String(shipDataModel.design.beam.m * _GPS_FEET_PER_METER, 1) + "'" : String("--"))).c_str());
  }

  void init_vesselScreen() {
    vesselScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    vesselScreen.init_cb = lv_vessel_display;
    vesselScreen.update_cb = vessel_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef UI_VESSEL_INFO_H
#define UI_VESSEL_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

  /*
    TODO: Use Rest Api 

    HTTP GET http://lysmarine:3000/signalk/v1/api/vessels/self/design/draft/value

    and so on
    
  */

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
    lv_label_set_text_static(main_label, "VESSEL  #0080ff " LV_SYMBOL_LIST " #");

    lv_obj_t *vessel_name_l = lv_label_create(parent);
    lv_obj_align(vessel_name_l, LV_ALIGN_TOP_LEFT, 10, 30);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_name_l, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_name_l, "Name: ");

    vessel_name_label = lv_label_create(parent);
    lv_obj_align(vessel_name_label, LV_ALIGN_TOP_LEFT, 100, 30);
    lv_label_set_long_mode(vessel_name_label, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(vessel_name_label, 180);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_name_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_name_label, "");

    vessel_mmsi_label = lv_label_create(parent);
    lv_obj_align(vessel_mmsi_label, LV_ALIGN_TOP_LEFT, 10, 60);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_mmsi_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_mmsi_label, "MMSI:");

    vessel_length_label = lv_label_create(parent);
    lv_obj_align(vessel_length_label, LV_ALIGN_TOP_LEFT, 10, 90);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_length_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_length_label, "Length:");

    vessel_draft_label = lv_label_create(parent);
    lv_obj_align(vessel_draft_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_draft_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_draft_label, "Draft:");

    vessel_beam_label = lv_label_create(parent);
    lv_obj_align(vessel_beam_label, LV_ALIGN_TOP_LEFT, 10, 150);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_beam_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_beam_label, "Beam:");

    vessel_air_height_label = lv_label_create(parent);
    lv_obj_align(vessel_air_height_label, LV_ALIGN_TOP_LEFT, 10, 180);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_air_height_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_air_height_label, "Air Height:");

    vessel_nav_status_label = lv_label_create(parent);
    lv_obj_align(vessel_nav_status_label, LV_ALIGN_TOP_LEFT, 10, 210);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(vessel_nav_status_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(vessel_nav_status_label, "Nav State:");
  }

  String navStateToStr(nav_state_e st) {
    switch (st) {
      case nav_state_e::NS_MOORED:
        return "moored";
      case nav_state_e::NS_SAILING:
        return "sailing";
      case nav_state_e::NS_MOTORING:
        return "motoring";
      case nav_state_e::NS_ANCHORED:
        return "anchored";
      default:
        return "--";
    }
  }

  static void vessel_update_cb() {
    lv_label_set_text(vessel_nav_status_label,
                      (String(F("Nav State:       "))
                       += (fresh(shipDataModel.navigation.state.age, 600000)
                            ? (navStateToStr(shipDataModel.navigation.state.st))
                            : String("--")))
                        .c_str());
    lv_label_set_text(vessel_name_label,
                      ((isSet(shipDataModel.vessel.name) ? String(shipDataModel.vessel.name) : String("--")))
                        .c_str());
    lv_label_set_text(vessel_mmsi_label,
                      (String(F("MMSI:     ")) += (isSet(shipDataModel.vessel.mmsi) ? String(shipDataModel.vessel.mmsi) : String("--")))
                        .c_str());
    lv_label_set_text(vessel_length_label,
                      (String(F("Length:            ")) += (shipDataModel.design.length.m > 0 ? String(shipDataModel.design.length.m * _GPS_FEET_PER_METER, 1) += "'" : String("--")))
                        .c_str());
    lv_label_set_text(vessel_draft_label,
                      (String(F("Draft:                ")) += (shipDataModel.design.draft.m > 0 ? String(shipDataModel.design.draft.m * _GPS_FEET_PER_METER, 1) += "'" : String("--")))
                        .c_str());
    lv_label_set_text(vessel_beam_label,
                      (String(F("Beam:               ")) += (shipDataModel.design.beam.m > 0 ? String(shipDataModel.design.beam.m * _GPS_FEET_PER_METER, 1) += "'" : String("--")))
                        .c_str());
    lv_label_set_text(vessel_air_height_label,
                      (String(F("Air Height:      ")) += (shipDataModel.design.air_height.m > 0 ? String(shipDataModel.design.air_height.m * _GPS_FEET_PER_METER, 1) += "'" : String("--")))
                        .c_str());
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

#ifndef UI_DEPTH_H
#define UI_DEPTH_H

#ifdef __cplusplus
extern "C" {
#endif

  lv_updatable_screen_t depthScreen;

  static lv_obj_t *dbt_label;
  static lv_obj_t *dbk_label;
  static lv_obj_t *dbs_label;
  static lv_obj_t *depth_gradient_label;
  static lv_obj_t *d_heel_label;

  /**
   * A depth display 
   */
  static void lv_depth_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_recolor(main_label, true);
    lv_label_set_text_static(main_label, "DEPTH  #0080ff " LV_SYMBOL_DOWNLOAD " #");

    dbt_label = lv_label_create(parent);
    lv_obj_align(dbt_label, LV_ALIGN_TOP_LEFT, 10, 40);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(dbt_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(dbt_label, "DBT (ft):          --");

    dbk_label = lv_label_create(parent);
    lv_obj_align(dbk_label, LV_ALIGN_TOP_LEFT, 10, 80);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(dbk_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(dbk_label, "DBK (ft):          --");

    dbs_label = lv_label_create(parent);
    lv_obj_align(dbs_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(dbs_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(dbs_label, "DBS (ft):          --");

    depth_gradient_label = lv_label_create(parent);
    lv_obj_align(depth_gradient_label, LV_ALIGN_TOP_LEFT, 10, 160);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(depth_gradient_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(depth_gradient_label, "Gradient:        --");

    d_heel_label = lv_label_create(parent);
    lv_obj_align(d_heel_label, LV_ALIGN_TOP_LEFT, 10, 200);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(d_heel_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(d_heel_label, "Heel:                 --");
  }

  static void depth_update_cb() {
    lv_label_set_text(dbt_label,
                      (String("DBT (ft):          ")
                       += (fresh(shipDataModel.environment.depth.below_transducer.age)
                            ? String(shipDataModel.environment.depth.below_transducer.m * _GPS_FEET_PER_METER, 1)
                            : String("--")))
                        .c_str());
    lv_label_set_text(dbk_label,
                      (String("DBK (ft):          ")
                       += (fresh(shipDataModel.environment.depth.below_keel.age)
                            ? String(shipDataModel.environment.depth.below_keel.m * _GPS_FEET_PER_METER, 1)
                            : String("--")))
                        .c_str());
    lv_label_set_text(dbs_label,
                      (String("DBS (ft):          ")
                       += (fresh(shipDataModel.environment.depth.below_surface.age)
                            ? String(shipDataModel.environment.depth.below_surface.m * _GPS_FEET_PER_METER, 1)
                            : String("--")))
                        .c_str());
    lv_label_set_text(d_heel_label,
                      (String("Heel:                 ")
                       += (fresh(shipDataModel.navigation.attitude.heel.age)
                            ? String(shipDataModel.navigation.attitude.heel.deg, 1) += String(LV_SYMBOL_DEGREES)
                            : String("--")))
                           .c_str());
    lv_label_set_text(depth_gradient_label,
                      (String("Gradient:        ")
                       += (fresh(shipDataModel.environment.depth_gradient.age)
                            ? String(shipDataModel.environment.depth_gradient.deg, 1) += String(LV_SYMBOL_DEGREES)
                            : String("--")))
                           .c_str());
  }

  void init_depthScreen() {
    depthScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    depthScreen.init_cb = lv_depth_display;
    depthScreen.update_cb = depth_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

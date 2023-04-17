#ifndef UI_WEATHER_H
#define UI_WEATHER_H

#ifdef __cplusplus
extern "C" {
#endif

  /*

   Look at:

   https://github.com/fandonov/weatherstation
   http://www.beteljuice.co.uk/zambretti/forecast.html

  */

  lv_updatable_screen_t weatherScreen;

  static lv_obj_t *weather_time_label;
  static lv_obj_t *weather_sunrise_label;
  static lv_obj_t *weather_sunset_label;
  static lv_obj_t *weather_twilight_begin_label;
  static lv_obj_t *weather_twilight_end_label;

  /**
   * A weather display 
   */
  void lv_weather_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_text(main_label, "Weather  " LV_SYMBOL_IMAGE);

    weather_time_label = lv_label_create(parent);
    lv_obj_align(weather_time_label, LV_ALIGN_TOP_LEFT, 10, 40);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_time_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(weather_time_label, "UTC Time:         --");

    weather_sunrise_label = lv_label_create(parent);
    lv_obj_align(weather_sunrise_label, LV_ALIGN_TOP_LEFT, 10, 70);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_sunrise_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(weather_sunrise_label, "Sunrise:          --");

    weather_sunset_label = lv_label_create(parent);
    lv_obj_align(weather_sunset_label, LV_ALIGN_TOP_LEFT, 10, 100);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_sunset_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(weather_sunset_label, "Sunset:          --");

    weather_twilight_begin_label = lv_label_create(parent);
    lv_obj_align(weather_twilight_begin_label, LV_ALIGN_TOP_LEFT, 10, 130);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_twilight_begin_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(weather_twilight_begin_label, "Twilight Start:        ---");

    weather_twilight_end_label = lv_label_create(parent);
    lv_obj_align(weather_twilight_end_label, LV_ALIGN_TOP_LEFT, 10, 130);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_twilight_end_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text(weather_twilight_end_label, "Twilight End:        ---");
  }

  static String hour_format(float hr) {
    char bufferTime[16];
    int h = hr;
    int m = (hr - h) * 60;
    int s = ((hr - h) * 60 - m) * 60;
    sprintf(bufferTime, "%02d:%02d:%02d", h, m, s);
    return String(bufferTime);
  }

  static void weather_update_cb() {
    lv_label_set_text(weather_time_label,
                      (String("UTC Time:          ")
                       + (fresh(shipDataModel.environment.time_gps.age) ? time_format(shipDataModel.environment.time_gps.t) : String("--")))
                        .c_str());
    lv_label_set_text(weather_sunrise_label,
                      (String("Sunrise:              ")
                       + (fresh(shipDataModel.environment.sunrise.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.sunrise.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_sunset_label,
                      (String("Sunset:               ")
                       + (fresh(shipDataModel.environment.sunset.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.sunset.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_twilight_begin_label,
                      (String("Twilight Start:               ")
                       + (fresh(shipDataModel.environment.nautical_twilight_start.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.nautical_twilight_start.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_twilight_end_label,
                      (String("Twilight End:               ")
                       + (fresh(shipDataModel.environment.nautical_twilight_end.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.nautical_twilight_end.hr) : String("--")))
                        .c_str());
  }

  void init_weatherScreen() {
    weatherScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    lv_weather_display(weatherScreen.screen);
    weatherScreen.update_cb = weather_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

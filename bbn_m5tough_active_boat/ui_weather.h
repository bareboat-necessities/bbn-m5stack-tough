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
  static lv_obj_t *weather_twilight_begin_label;
  static lv_obj_t *weather_sunrise_label;
  static lv_obj_t *weather_daylight_label;
  static lv_obj_t *weather_sunset_label;
  static lv_obj_t *weather_twilight_label;
  static lv_obj_t *weather_twilight_end_label;

  /**
   * A weather display 
   */
  static void lv_weather_display(lv_obj_t *parent) {

    lv_obj_t *main_label = lv_label_create(parent);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -105);
    lv_label_set_recolor(main_label, true);
    lv_label_set_text_static(main_label, "SUN  #0080ff " LV_SYMBOL_IMAGE " #");

    weather_time_label = lv_label_create(parent);
    lv_obj_align(weather_time_label, LV_ALIGN_TOP_LEFT, 10, 30);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_time_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_time_label, "UTC Time:");

    weather_twilight_begin_label = lv_label_create(parent);
    lv_obj_align(weather_twilight_begin_label, LV_ALIGN_TOP_LEFT, 10, 60);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_twilight_begin_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_twilight_begin_label, "Twilight Start:");

    weather_sunrise_label = lv_label_create(parent);
    lv_obj_align(weather_sunrise_label, LV_ALIGN_TOP_LEFT, 10, 90);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_sunrise_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_sunrise_label, "Sunrise:");

    weather_daylight_label = lv_label_create(parent);
    lv_obj_align(weather_daylight_label, LV_ALIGN_TOP_LEFT, 10, 120);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_daylight_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_daylight_label, "Day Duration:");

    weather_sunset_label = lv_label_create(parent);
    lv_obj_align(weather_sunset_label, LV_ALIGN_TOP_LEFT, 10, 150);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_sunset_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_sunset_label, "Sunset:");

    weather_twilight_label = lv_label_create(parent);
    lv_obj_align(weather_twilight_label, LV_ALIGN_TOP_LEFT, 10, 180);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_twilight_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_twilight_label, "Twilight Duration:");

    weather_twilight_end_label = lv_label_create(parent);
    lv_obj_align(weather_twilight_end_label, LV_ALIGN_TOP_LEFT, 10, 210);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(weather_twilight_end_label, &lv_font_montserrat_20, 0);
#endif
    lv_label_set_text_static(weather_twilight_end_label, "Twilight End:");
  }

  static String hour_format(float hr) {
    static char bufferTime[16];
    if (hr < 0) {
      hr = 24 + hr;
    }
    int h = hr;
    int m = (hr - h) * 60;
    int s = ((hr - h) * 60 - m) * 60;
    sprintf(bufferTime, "%02d:%02d:%02d", h, m, s);
    return String(bufferTime);
  }

  static void weather_update_cb() {
    lv_label_set_text(weather_time_label,
                      (String(F("UTC Time:                   "))
                       += (fresh(shipDataModel.environment.time_gps.age) ? time_format(shipDataModel.environment.time_gps.t) : String("--")))
                        .c_str());
    lv_label_set_text(weather_twilight_begin_label,
                      (String(F("Twilight Start:           "))
                       += (fresh(shipDataModel.environment.nautical_twilight_start.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.nautical_twilight_start.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_sunrise_label,
                      (String(F("Sunrise:                       "))
                       += (fresh(shipDataModel.environment.sunrise.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.sunrise.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_daylight_label,
                      (String(F("Day Duration:           "))
                       += (fresh(shipDataModel.environment.daylight_duration.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.daylight_duration.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_sunset_label,
                      (String(F("Sunset:                        "))
                       += (fresh(shipDataModel.environment.sunset.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.sunset.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_twilight_label,
                      (String(F("Twilight Duration:  "))
                       += (fresh(shipDataModel.environment.nautical_twilight_duration.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.nautical_twilight_duration.hr) : String("--")))
                        .c_str());
    lv_label_set_text(weather_twilight_end_label,
                      (String(F("Twilight End:            "))
                       += (fresh(shipDataModel.environment.nautical_twilight_end.age, TWO_MINUTES) ? hour_format(shipDataModel.environment.nautical_twilight_end.hr) : String("--")))
                        .c_str());
  }

  void init_weatherScreen() {
    weatherScreen.screen = lv_obj_create(NULL);  // Creates a Screen object
    weatherScreen.init_cb = lv_weather_display;
    weatherScreen.update_cb = weather_update_cb;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

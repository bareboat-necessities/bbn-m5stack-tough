#ifndef DERIVED_DATA_H
#define DERIVED_DATA_H

/*
 Also look at https://github.com/SignalK/signalk-derived-data
*/

#include "leeway.h"

#ifdef __cplusplus
extern "C" {
#endif

  static void derive_data() {
    // derive magnetic variation
    if (fresh(shipDataModel.navigation.position.lat.age)
        && fresh(shipDataModel.navigation.position.lon.age)) {
      M5.Rtc.GetDate(&RTCdate);
      float longitude = shipDataModel.navigation.position.lon.deg;
      float latitude = shipDataModel.navigation.position.lat.deg;
      float mag_var_deg = myDeclination.magneticDeclination(latitude, longitude, RTCdate.Year % 100, 1 + RTCdate.Month, RTCdate.Date);
      shipDataModel.navigation.mag_var.deg = mag_var_deg;
      shipDataModel.navigation.mag_var.age = millis();
    }

    if (fresh(shipDataModel.navigation.course_over_ground_true.age)) {
      shipDataModel.navigation.course_over_ground_mag.deg = shipDataModel.navigation.course_over_ground_true.deg - shipDataModel.navigation.mag_var.deg;
      shipDataModel.navigation.course_over_ground_mag.age = millis();
    }
    if (fresh(shipDataModel.navigation.heading_mag.age)) {
      shipDataModel.navigation.heading_true.deg = shipDataModel.navigation.heading_mag.deg + shipDataModel.navigation.mag_var.deg;
      shipDataModel.navigation.heading_true.age = millis();
    }

    // ground wind calc
    if (fresh(shipDataModel.navigation.course_over_ground_true.age)
        && fresh(shipDataModel.navigation.speed_over_ground.age)
        && fresh(shipDataModel.navigation.heading_true.age)
        && fresh(shipDataModel.environment.wind.apparent_wind_speed.age)
        && fresh(shipDataModel.environment.wind.apparent_wind_angle.age)) {
      float heading_speed_over_ground = shipDataModel.navigation.speed_over_ground.kn * cos((shipDataModel.navigation.course_over_ground_true.deg - shipDataModel.navigation.heading_true.deg) * PI / 180.0);
      float head_wind = (-heading_speed_over_ground);
      float aws = shipDataModel.environment.wind.apparent_wind_speed.kn;
      float awa = shipDataModel.environment.wind.apparent_wind_angle.deg * PI / 180.0;
      float ground_wind_speed =
        sqrt(aws * aws + head_wind * head_wind + 2.0 * aws * head_wind * cos(awa));
      float ground_wind_angle_rad = acos((aws * cos(awa) + head_wind) / ground_wind_speed);
      if (awa < 0 || awa > PI) { // port side
        ground_wind_angle_rad = (-ground_wind_angle_rad);
      }
      float ground_wind_angle_deg = ground_wind_angle_rad * 180.0 / PI;
      float ground_wind_angle_true_deg = ground_wind_angle_deg + shipDataModel.navigation.heading_true.deg;
      if (ground_wind_angle_true_deg > 180.0) {
        ground_wind_angle_true_deg = ground_wind_angle_true_deg - 360.0;
      }
      shipDataModel.environment.wind.ground_wind_speed.kn = ground_wind_speed;
      shipDataModel.environment.wind.ground_wind_speed.age = millis();
      shipDataModel.environment.wind.ground_wind_angle_true.deg = ground_wind_angle_true_deg;
      shipDataModel.environment.wind.ground_wind_angle_true.age = millis();
    }

    // derive leeway
    if (fresh(shipDataModel.navigation.speed_through_water.age)
        && fresh(shipDataModel.navigation.attitude.heel.age)) {
      int k_factor = 12;  // TODO:
      shipDataModel.navigation.leeway.deg = leeway_deg(shipDataModel.navigation.attitude.heel.deg,
                                                       shipDataModel.navigation.speed_through_water.kn, k_factor);
      shipDataModel.navigation.leeway.age = millis();
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

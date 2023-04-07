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
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

/******************************************************************************
WMM_Tinier.cpp
WMM_Tinier Arduino Library C++ source
David Armstrong
Version 1.0.1 - October 15, 2022
https://github.com/DavidArmstrong/WMM_Tinier

Resources:
Uses math.h for math function
Refer to https://github.com/miniwinwm/WMM_Tiny

Development environment specifics:
Arduino IDE 1.8.15

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example.
Distributed as-is; no warranty is given.
******************************************************************************/

// include this library's description file
#include "WMM_Tinier.h"
#include "wmm.c"

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Public Methods //////////////////////////////////////////////////////////
// Start by doing any setup, and verifying that floats are supported
boolean WMM_Tinier::begin(void) {
  wmm_init();
  return true;
}

float WMM_Tinier::decimalDegrees(int degrees, int minutes, float seconds) {
  int sign = 1;
  if (degrees < 0) {
    degrees = -degrees;
    sign = -1;
  }
  if (minutes < 0) {
    minutes = -minutes;
    sign = -1;
  }
  if (seconds < 0) {
    seconds = -seconds;
    sign = -1;
  }
  float decDeg = degrees + (minutes / 60.0) + (seconds / 3600.);
  return decDeg * sign;
}

void WMM_Tinier::printDegMinSecs(float n) {
  boolean sign = (n < 0.);
  if (sign) n = -n;
  long lsec = n * 360000.0;
  long deg = lsec / 360000;
  long min = (lsec - (deg * 360000)) / 6000;
  float secs = (lsec - (deg * 360000) - (min * 6000)) / 100.;
  if (sign) Serial.print("-");
  Serial.print(deg);
  Serial.print(":");
  Serial.print(min);
  Serial.print(":");
  Serial.print(abs(secs));
  Serial.print(" ");
}

float WMM_Tinier::magneticDeclination(float Latitude, float Longitude, uint8_t year, uint8_t month, uint8_t day) {
  //wmm_init();
  float wmm_date = wmm_get_date(year, month, day);
  float variation;
  E0000(Latitude, Longitude, wmm_date, &variation);
  return variation;
}

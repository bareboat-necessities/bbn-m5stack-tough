/******************************************************************************
WMM_Tinier.h
WMM_Tinier Arduino Library Header File
David Armstrong
Version 1.0.0 - August 6, 2021
https://github.com/DavidArmstrong/WMM_Tinier

This file prototypes the WMM_Tinier class, as implemented in WMM_Tinier.cpp

Resources:
Uses math.h for math functions
Refer to https://github.com/miniwinwm/WMM_Tiny

Development environment specifics:
Arduino IDE 1.8.15

This code is released under the [MIT License](http://opensource.org/licenses/MIT)
Please review the LICENSE.md file included with this example.
Distributed as-is; no warranty is given.

******************************************************************************/

// ensure this library description is only included once
#ifndef __WMM_Tinier_h
#define __WMM_Tinier_h

//Uncomment the following line for debugging output
//#define debug_WMM_Tinier

#include <stdint.h>
#include <math.h>
#include "wmm.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Structure to hold data
// We need to populate this when we calculate data
struct WMM_TinierData {
public:
  float longitude;
  float latitude;
  float date;
  float declination;
};

// Sidereal_Planets library description
class WMM_Tinier {
  // user-accessible "public" interface
public:
  WMM_TinierData spData;
  boolean begin(void);
  float decimalDegrees(int degrees, int minutes, float seconds);
  void printDegMinSecs(float n);
  float magneticDeclination(float Latitude, float Longitude, uint8_t year, uint8_t month, uint8_t day);

  // library-accessible "private" interface
private:
};
#endif

#ifndef LEEWAY_H
#define LEEWAY_H

/*
 Also look at https://github.com/SignalK/signalk-derived-data
*/

#ifdef __cplusplus
extern "C" {
#endif

  static float leeway_angle(float hdt, float cogt) {
    return abs(hdt - cogt);
  }
 
  // Empirical formula for leeway
  // Leeway (deg) is angle to adjust heading to maintain constant COG (assuming no current)
  // kFactor - Leeway correlation constant, typically from 9 to 16 (9 for super racer)
  // heel_deg - heel angle in degrees
  // stw_kn - true wind in knots
  static float leeway(float heel_deg, float stw_kn, int kFactor = 12) {
    float leeway_deg = kFactor * heel_deg / (stw_kn * stw_kn);
    retun leeway_deg;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

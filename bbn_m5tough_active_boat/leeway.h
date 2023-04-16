#ifndef LEEWAY_H
#define LEEWAY_H

/*
 Also look at https://github.com/SignalK/signalk-derived-data
*/

#ifdef __cplusplus
extern "C" {
#endif
 
  // Empirical formula for leeway.
  // Leeway (deg) is angle to adjust heading to maintain constant COG (assuming no current)
  // k_factor - leeway correlation constant (is boat and boat load specific constant), typically from 9 to 16 (9 for super racer)
  // heel_deg - heel angle in degrees
  // stw_kn - speed through water (aka LOG or SPD) in knots
  static float leeway_deg(float heel_deg, float stw_kn, int k_factor = 12) {
    float leeway_deg = k_factor * heel_deg / (stw_kn * stw_kn);
    return leeway_deg;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

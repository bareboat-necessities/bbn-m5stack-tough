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

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

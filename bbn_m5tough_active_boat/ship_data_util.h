#ifndef SHIP_DATA_UTIL_H
#define SHIP_DATA_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

  #define TWO_MINUTES 120000
  #define LONG_EXPIRE_TO 172800000

  bool fresh(unsigned long age, unsigned long limit = 5000) {
    return millis() - age < limit && age != 0;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

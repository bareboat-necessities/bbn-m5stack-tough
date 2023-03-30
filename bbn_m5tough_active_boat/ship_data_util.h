#ifndef SHIP_DATA_UTIL_H
#define SHIP_DATA_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

  bool fresh(unsigned long age) {
    return millis() - age < 5000;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

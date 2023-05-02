#ifndef HW_RTC_H
#define HW_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

  RTC_TimeTypeDef RTCtime;
  RTC_DateTypeDef RTCdate;

  void rtc_begin() {
    M5.Rtc.begin();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

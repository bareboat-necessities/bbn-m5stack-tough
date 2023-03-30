#ifndef NET_NTP_TIME_H
#define NET_NTP_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

  const char* ntpServer = "pool.ntp.org";
  const long utcOffset_sec = 0;
  const int daylightOffset_sec = 0;  //3600;

  void setupTime() {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      RTCdate.Year = timeinfo.tm_year;
      RTCdate.Month = timeinfo.tm_mon;
      RTCdate.Date = timeinfo.tm_mday;
      M5.Rtc.SetDate(&RTCdate);

      RTCtime.Hours = timeinfo.tm_hour;
      RTCtime.Minutes = timeinfo.tm_min;
      RTCtime.Seconds = timeinfo.tm_sec;
      M5.Rtc.SetTime(&RTCtime);  // writes the  time to the (RTC) real time clock.
    }
  }

  void init_dateTime() {
    configTime(utcOffset_sec, daylightOffset_sec, ntpServer);
    setupTime(); 
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

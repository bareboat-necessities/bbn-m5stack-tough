#include <M5Tough.h>

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

char timeStrbuff[64];

void flushTime() {
  M5.Rtc.GetTime(&RTCtime);  //Gets the time from the real-time clock.
  M5.Rtc.GetDate(&RTCDate);
  sprintf(timeStrbuff, "%d/%02d/%02d %02d:%02d:%02d",  //Stores real-time time and date data to timeStrbuff.
          RTCDate.Year, RTCDate.Month, RTCDate.Date,
          RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
  M5.lcd.setCursor(10, 100);  //Move the cursor position to (x,y).
  M5.Lcd.println(timeStrbuff);
}

void setupTime() {
  RTCtime.Hours = 12;  //Set the time.
  RTCtime.Minutes = 55;
  RTCtime.Seconds = 33;
  M5.Rtc.SetTime(&RTCtime);  //and writes the set time to the real time clock.

  RTCDate.Year = 2023;  //Set the date.
  RTCDate.Month = 3;
  RTCDate.Date = 24;
  M5.Rtc.SetDate(&RTCDate);
}

/* 
  After M5Tough is started or reset the program in the setUp () function will be run, 
  and this part will only be run once.
 */
void setup() {
  M5.begin();  //Init M5Tough.
  setupTime();
}

/* 
  After the program in setup() runs, it runs the program in loop()
  The loop() function is called from an infinite loop in which the program runs
*/
void loop() {
  flushTime();
  delay(1000);
}

#include <M5Tough.h>

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCdate;

char timeStrbuff[64];

void flushTime() {
  M5.Rtc.GetTime(&RTCtime);  //Gets the time from the real-time clock.
  M5.Rtc.GetDate(&RTCdate);
  sprintf(timeStrbuff, "%d/%02d/%02d %02d:%02d:%02d",  //Stores real-time time and date data to timeStrbuff.
          RTCdate.Year, RTCdate.Month, RTCdate.Date,
          RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
  M5.lcd.setCursor(10, 100);  //Move the cursor position to (x,y).
  M5.Lcd.println(timeStrbuff);
}

void setupTime() {
  //RTCtime.Hours = 12;  //Set the time.
  //RTCtime.Minutes = 55;
  //RTCtime.Seconds = 33;
  //M5.Rtc.SetTime(&RTCtime);  //and writes the set time to the real time clock.

  //RTCdate.Year = 2023;  //Set the date.
  //RTCdate.Month = 3;
  //RTCdate.Date = 24;
  //M5.Rtc.SetDate(&RTCdate);
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

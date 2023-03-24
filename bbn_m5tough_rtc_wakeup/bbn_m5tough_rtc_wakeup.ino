#include <M5Tough.h>

//RTC_TimeTypeDef RTCtime;
RTC_TimeTypeDef RTCtime_Now;

char timeStrBuf[64];

void setup() {
  M5.begin();
  M5.Lcd.println("I will wait for 10 seconds before a shutdown.");
  M5.Lcd.println("Then I will wake up after 5 seconds.");
  delay(10000);
  M5.shutdown(5);
  //M5.shutdown(RTCtime); //Turn off the power and wake up at the specified time.
}

void loop() {
  M5.update();
  M5.Lcd.setCursor(0, 140);
  M5.Rtc.GetTime(&RTCtime_Now);                          //Gets the current time.
  sprintf(timeStrBuf, "RTC Time Now is %02d:%02d:%02d",  //Stores real-time time data to timeStrBuf.
          RTCtime_Now.Hours, RTCtime_Now.Minutes, RTCtime_Now.Seconds);
  M5.Lcd.println(timeStrBuf);  //Screen printing output timeStrBuf.
}

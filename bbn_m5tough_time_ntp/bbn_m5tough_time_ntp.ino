#include <M5Tough.h>
#include <WiFi.h>
#include <time.h>
#include <Preferences.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

const char* ntpServer = "pool.ntp.org";
const long utcOffset_sec = 0;
const int daylightOffset_sec = 0; //3600;

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCdate;

void printUTCTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {  //Return 1 when the time is successfully obtained.
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  M5.Lcd.println(&timeinfo, "UTC %A, %B %d \n%Y %H:%M:%S");  //Screen prints date and time.
}

void setup() {
  M5.begin();  //Init M5Tough.

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  M5.Lcd.printf("\nConnecting to %s", wifi_ssid.c_str());
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nCONNECTED!");
  delay(3000);
  configTime(utcOffset_sec, daylightOffset_sec, ntpServer);  //init and get the time.
  setupTime();
  printUTCTime();
  delay(20);
}

void setupTime() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) { 
    RTCtime.Hours = timeinfo.tm_hour;
    RTCtime.Minutes = timeinfo.tm_min;
    RTCtime.Seconds = timeinfo.tm_sec;
    M5.Rtc.SetTime(&RTCtime);  // writes the  time to the (RTC) real time clock.

    RTCdate.Year = timeinfo.tm_year; 
    RTCdate.Month = timeinfo.tm_mon;
    RTCdate.Date = timeinfo.tm_mday;
    M5.Rtc.SetDate(&RTCdate);
  }
}

void loop() {
  delay(1000);
  M5.Lcd.setCursor(0, 47);
  printUTCTime();
}

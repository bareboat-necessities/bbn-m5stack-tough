#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketClient.h>  // https://github.com/u0078867/Arduino-Websocket-Fast/ (rename String.h reference to string.h, remove Base64.cpp)
#include <ArduinoJson.h>
#include <Preferences.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

void setup() {
  M5.begin();
  M5.Lcd.setTextWrap(true, true);
  M5.Lcd.setTextColor(YELLOW);

  Serial.begin(115200);
  while (!Serial) continue;
  delay(500);

  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  int attemptsCount = 30;
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(wifi_ssid);
    status = WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    // wait .5 seconds for connection:
    delay(500);
    M5.Lcd.print(".");
    attemptsCount--;
    if (attemptsCount == 0) return;
  }
}

void loop() {
}

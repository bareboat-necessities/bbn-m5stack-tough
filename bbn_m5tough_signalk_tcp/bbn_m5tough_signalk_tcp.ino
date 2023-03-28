#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

#include "ship_data_model.h"

ship_data_t ShipDataModel;

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient skClient;

using namespace reactesp;
ReactESP app;

#include "signalk_tcp.h"

void setup() {
  M5.begin();
  M5.Lcd.setTextWrap(true, true);
  M5.Lcd.setTextColor(YELLOW);

  Serial.begin(115200);
  while (!Serial) continue;
  delay(500);

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  int attemptsCount = 30;
  int status = WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(wifi_ssid);
    status = WiFi.status();
    // wait .5 seconds for connection:
    delay(500);
    M5.Lcd.print(".");
    attemptsCount--;
    if (attemptsCount == 0) return;
  }
  if (status == WL_CONNECTED) {
    M5.Lcd.println("");
    M5.Lcd.println("Connected to " + wifi_ssid);
  }

  const char* host = "192.168.1.34";  //"lysmarine";
  int port = 8375;                    // SignalK TCP

  // Connect to the SignalK TCP server
  if (skClient.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);

    signalk_subscribe(skClient);

  } else {
    M5.Lcd.println("Connection failed.");
  }
}

void loop() {
  M5.update();
  app.tick();
}

#include <M5Tough.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <mdns.h>

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

  if (!MDNS.begin("ESP32_Browser")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  browseService("http", "tcp");
  delay(100);
  browseService("nmea-0183", "tcp");
  delay(100);
  browseService("signalk-http", "tcp");
  delay(100);
  browseService("signalk-tcp", "tcp");
  delay(100);
  browseService("signalk-ws", "tcp");
  delay(100);
  browseService("pypilot", "tcp");
  delay(100);
  browseService("mopidy-http", "tcp");
  delay(100);
}

void loop() {
  M5.update();
  delay(5);
}

void browseService(const char* service, const char* proto) {
  M5.Lcd.printf("Scan _%s._%s.local. ... ", service, proto);
  int n = MDNS.queryService(service, proto);
  if (n == 0) {
    M5.Lcd.println("no found");
  } else {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print(n);
    M5.Lcd.println(" found");
    M5.Lcd.setTextColor(YELLOW);
    for (int i = 0; i < n; ++i) {
      // Print details for each service found
      M5.Lcd.print(" ");
      M5.Lcd.print(i + 1);
      M5.Lcd.print(": ");
      M5.Lcd.print(MDNS.hostname(i));
      M5.Lcd.print(" (");
      M5.Lcd.print(MDNS.IP(i));
      M5.Lcd.print(":");
      M5.Lcd.print(MDNS.port(i));
      M5.Lcd.println(")");
    }
  }
  M5.Lcd.println();
}

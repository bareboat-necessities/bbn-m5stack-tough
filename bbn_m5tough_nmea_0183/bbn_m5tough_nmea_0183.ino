#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient nmeaClient;

using namespace reactesp;
ReactESP app;

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
  int port = 10110;                   // NMEA 0183

  // Connect to the SignalK TCP server
  if (nmeaClient.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);

    nmea_subscribe(nmeaClient);

  } else {
    M5.Lcd.println("Connection failed.");
    return;
  }
}

void loop() {
  M5.update();
  app.tick();
}

int samples = 20;

void nmea_subscribe(WiFiClient& client) {
  delay(50);
  String dataFeed = client.readStringUntil('\n');
  M5.Lcd.println(dataFeed);
  delay(50);

  app.onAvailable(client, [samples, &client]() {
    while (client.connected() && client.available()) {
      bool found = nmea_parse(client);
      if (found) {
        samples--;
        if (samples <= 0) {
          client.stop();
        }
      }
    }
    delay(1);
  });
}

bool nmea_parse(Stream& stream) {
  return true;
}


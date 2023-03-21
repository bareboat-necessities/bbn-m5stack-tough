#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient client;

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
  if (client.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);

    delay(50);
    String dataFeed = client.readStringUntil('\n');
    M5.Lcd.println(dataFeed);
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
    client.println(data);
    client.flush();
    delay(50);

    int samples = 20;
    while (client.connected() && samples > 0) {
      if (client.available()) {
        dataFeed = client.readStringUntil('\n');
        if (dataFeed.length() > 0) {
          //M5.Lcd.println(dataFeed);
          String parsed = handleReceivedMessage(dataFeed);
          if (parsed.length() > 0) {
            M5.Lcd.println(parsed);
            samples--;
          }
        }
      } else {
        delay(1);
      }
    }
  } else {
    M5.Lcd.println("Connection failed.");
    return;
  }
}

void loop() {
  M5.update();
}

String updatedValue;

String handleReceivedMessage(String message) {
  StaticJsonDocument<4096> doc;
  DeserializationError err = deserializeJson(doc, message);
  // Parse succeeded?
  if (err) {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(err.c_str());
    return "INVALID";
  }
  JsonObject obj = doc.as<JsonObject>();

  JsonObject updates = obj["updates"][0]["values"][0];
  if (updates.containsKey("path")) {
    const char* path = updates["path"].as<const char*>();
    updatedValue = "";
    if (path != NULL) {
      updatedValue = String(path) + ": ";
      if (updates.containsKey("value")) {
        JsonVariant value = updates["value"];
        if (value.is<int>()) {
          updatedValue = updatedValue + " int: ";
        } else if (value.is<float>()) {
          updatedValue = updatedValue + " float: ";
        } else if (value.is<const char*>()) {
          updatedValue = updatedValue + " str: ";
        } else if (value.is<boolean>()) {
          updatedValue = updatedValue + " bool: ";
        }
        serializeJson(value, updatedValue);
      }
    }
  }
  return updatedValue;
}
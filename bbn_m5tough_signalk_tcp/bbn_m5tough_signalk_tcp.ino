#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient skClient;

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
  int port = 8375;                    // SignalK TCP

  // Connect to the SignalK TCP server
  if (skClient.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);

    signalk_subscribe(skClient);

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

void signalk_subscribe(WiFiClient& client) {
  delay(50);
  String dataFeed = client.readStringUntil('\n');
  M5.Lcd.println(dataFeed);
  const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
  client.println(data);
  client.flush();
  delay(50);

  app.onAvailable(client, [samples, &client]() {
    while (client.connected() && client.available()) {
      bool found = signalk_parse(client);
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

bool signalk_parse(Stream& stream) {
  bool found = false;
  DynamicJsonDocument doc(4096);
  DeserializationError err = deserializeJson(doc, stream);
  // Parse succeeded?
  if (err) {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(err.c_str());
    return found;
  }
  JsonObject obj = doc.as<JsonObject>();

  auto update_value = [&](String& path, size_t& u_idx, size_t& v_idx, JsonVariant& value) {
    String updatedValue = path;
    if (value.is<float>()) {
      updatedValue = updatedValue + " float: ";
    } else if (value.is<int>()) {
      updatedValue = updatedValue + " int: ";
    } else if (value.is<const char*>()) {
      updatedValue = updatedValue + " str: ";
    } else if (value.is<boolean>()) {
      updatedValue = updatedValue + " bool: ";
    } else {
      updatedValue = updatedValue + " ";
    }
    serializeJson(value, updatedValue);
    M5.Lcd.println(updatedValue);
  };

  JsonArray updates = obj["updates"];
  if (updates != NULL) {
    for (size_t i_u = 0; i_u < updates.size(); i_u++) {
      JsonObject update = updates[i_u];
      if (update != NULL) {
        JsonArray values = update["values"];
        if (values != NULL) {
          for (size_t i_v = 0; i_v < values.size(); i_v++) {
            JsonObject valueObj = values[i_v];
            if (valueObj != NULL) {
              if (valueObj.containsKey("path")) {
                String path = valueObj["path"].as<const char*>();
                if (path != NULL) {
                  JsonVariant value = valueObj["value"];
                  update_value(path, i_u, i_v, value);
                  found = true;
                }
              }
            }
          }
        }
      }
    }
  }

  return found;
}

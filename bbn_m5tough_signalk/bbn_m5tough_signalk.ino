#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

#include <WebSocketClient.h>  // https://github.com/u0078867/Arduino-Websocket-Fast/ (rename String.h reference to string.h, remove Base64.cpp)
#include <ArduinoJson.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient client;
WebSocketClient webSocketClient;

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

  webSocketClient.path = "/signalk/v1/stream?subcribe=all";
  webSocketClient.host = "192.168.1.34";  //"lysmarine";
  int port = 3000;

  // Connect to the websocket server
  if (client.connect(webSocketClient.host, port, 10000)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(webSocketClient.host);
  } else {
    M5.Lcd.println("Connection failed.");
    return;
  }

  // Handshake with the server
  if (webSocketClient.handshake(client)) {
    M5.Lcd.println("Handshake successful");
  } else {
    M5.Lcd.println("Handshake failed.");
    return;
  }

  String data = "{\"context\":\"*\",\"subscribe\":[{\"path\": \"*\", \"format\": \"delta\"}]}";
  webSocketClient.sendData(data);
  delay(1000);

  static String dataFeed;
  static int samples = 12;

  app.onAvailable(client, [samples, &dataFeed, &client]() {
    while (client.connected() && client.available() > 0) {
      webSocketClient.getData(dataFeed);
      if (dataFeed.length() > 0) {
        bool found = signalk_parse(dataFeed);
        if (found) {
          samples--;
          if (samples <= 0) {
            client.stop();
          }
          break;
        }
      }
    }
    delay(1);
  });
}

void loop() {
  M5.update();
  app.tick();
}

bool signalk_parse(String& str) {
  bool found = false;
  DynamicJsonDocument doc(4096);
  DeserializationError err = deserializeJson(doc, str);
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

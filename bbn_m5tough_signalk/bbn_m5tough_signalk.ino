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

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient client;
WebSocketClient webSocketClient;

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
  webSocketClient.host = "192.168.1.34"; //"lysmarine";
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

  String dataFeed;
  int samples = 30;
  while (client.connected() && samples > 0) {
    webSocketClient.getData(dataFeed);
    if (dataFeed.length() > 0) {
      String parsed = handleReceivedMessage(dataFeed);
      if (parsed.length() > 0) {
        M5.Lcd.println(parsed);
        samples--;
      }
    }
    delay(5);
  }
}

void loop() {
  M5.update();
}


String updatedValue;

String handleReceivedMessage(String message){
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
  JsonObject value_obj = updates["value"];
  const char* value = updates["value"];
  const char* path = updates["path"];

  updatedValue = "";
  if (path != NULL) {
    updatedValue = String(path) + ": ";
    if (strcmp(path, "navigation.position") == 0) {
      float lon = value_obj["longitude"];
      float lat = value_obj["latitude"];
      updatedValue = updatedValue + lat + " " + lon;
    }
    else if (strcmp(path, "navigation.gnss.satellitesInView") == 0) {
    } 
    else if (strncmp(path, "notifications.security.accessRequest", strlen("notifications.security.accessRequest")) == 0) {
    }
    else {
      updatedValue = updatedValue + String(value);
    }
  }
  return updatedValue;
}

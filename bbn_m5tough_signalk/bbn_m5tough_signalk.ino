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

  webSocketClient.path = "/signalk/v1/stream?subcribe=none";
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
}

void loop() {
  M5.update();
}

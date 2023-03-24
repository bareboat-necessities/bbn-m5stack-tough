#include <M5Tough.h>
#include <Preferences.h>
#include <WiFi.h>
#include <MQTT.h>  // https://github.com/256dpi/arduino-mqtt

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  M5.Lcd.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(1000);
  }

  M5.Lcd.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    M5.Lcd.print(".");
    delay(1000);
  }

  M5.Lcd.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  M5.Lcd.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  M5.begin();
  Serial.begin(115200);

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  M5.update();
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}

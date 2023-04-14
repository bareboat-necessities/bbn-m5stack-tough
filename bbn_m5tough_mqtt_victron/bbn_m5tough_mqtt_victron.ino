#include <M5Tough.h>
#include <Preferences.h>
#include <WiFi.h>
#include <MQTT.h>  // https://github.com/256dpi/arduino-mqtt

static Preferences preferences;
static String wifi_ssid;      // Store the name of the wireless network.
static String wifi_password;  // Store the password of the wireless network.

static WiFiClient net;
static MQTTClient client = MQTTClient(4096);

unsigned long lastMillis = 0;

void connect() {
  client.onMessage(messageReceived);
  client.setKeepAlive(10);
  //M5.Lcd.print("\nconnecting...");
  while (!client.connect("arduino" /* clientID */, "" /*user*/, "" /*password*/)) {
    M5.Lcd.print(".");
    delay(1000);
  }
  //M5.Lcd.println("\nconnected!");

  client.subscribe("N/+/+/#");
  client.publish("R/0242ac110002/system/0/Serial");
  //client.subscribe("N/#");
  //client.subscribe("N/0242ac110002/tank/#");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  if (topic.endsWith("/system/0/Serial")) {
    //client.publish("R/0242ac110002/system/0/Serial");
    //client.subscribe("N/0242ac110002/+/#");
    //client.subscribe("N/0242ac110002/#");
    //M5.Lcd.println("incoming: " + topic + " - " + payload);
  }
  if (topic == "N/0242ac110002/vebus/257/Ac/Out/L1/P") {
    M5.Lcd.println("incoming: " + topic + " - " + payload);
  }
  if (topic.indexOf("tank") > 0 /*topic.indexOf("L1/Power") > 0*/) {
    M5.Lcd.println("incoming: " + topic + " - " + payload);
  }
  //M5.Lcd.println("incoming: " + topic);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  M5.begin();
  Serial.begin(115200);

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  delay(10);
  M5.Lcd.print(wifi_ssid);
  M5.Lcd.print(" : ");
  M5.Lcd.println(wifi_password);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  M5.Lcd.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(350);
  }

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("192.168.1.64", 1883, net);

  connect();
}


void loop() {
  M5.update();
  client.loop();

  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 20000) {
    lastMillis = millis();
    client.publish("R/0242ac110002/system/0/Serial");
  }
}

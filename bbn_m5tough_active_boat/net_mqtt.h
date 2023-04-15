#ifndef NET_MQTT_H
#define NET_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

  /*

  Victron MQQT
  To start simulator wth docker https://github.com/victronenergy/venus-docker
  
  /simulate.sh --with-solarcharger --with-tanks p
  
  To see messages:
  mosquitto_sub -t 'N/#'

  See also: https://github.com/sbender9/signalk-venus-plugin/blob/master/index.js

*/

  typedef struct _victron_mqtt_context_t {
    String portalID;
    bool needs_id = true;
    unsigned long lastMillis = 0;
  } victron_mqtt_context_t;

  static victron_mqtt_context_t victronCtx;

  static void victron_mqtt_on_message(String& topic, String& payload) {
    if (topic.endsWith("/system/0/Serial") && victronCtx.needs_id) {
      String str = topic.substring(2);
      str.remove(str.indexOf("/"));
      victronCtx.portalID.clear();
      victronCtx.portalID += str;
      victronCtx.needs_id = false;
    }
    if (topic.indexOf("tank") > 0 /*topic.indexOf("L1/Power") > 0*/) {
      //M5.Lcd.println("incoming: " + topic + " - " + payload);
    }
    //M5.Lcd.println("incoming: " + topic);

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
  }

  void victron_mqtt_client_connect(MQTTClient& client) {
    client.onMessage(victron_mqtt_on_message);
    client.setKeepAlive(10);
    while (!client.connect("arduino" /*clientID*/, "" /*user*/, "" /*password*/)) {
      M5.Lcd.print(".");
      delay(1000);
    }
    M5.Lcd.println("\nMQTT connected!");

    client.subscribe("N/+/+/#");
  }

  void victron_mqtt_client_begin(MQTTClient& client, WiFiClient& net, const char* host, int port) {
    setKeepAlive(net);
    client.begin(host, port, net);
    victron_mqtt_client_connect(client);
  }

  void victron_mqtt_client_loop(MQTTClient& client) {
    client.loop();
    if (!client.connected()) {
      victron_mqtt_client_connect(client);
    }
    if (millis() - victronCtx.lastMillis > 20000) {
      victronCtx.lastMillis = millis();
      // keep alive
      if (victronCtx.portalID.length() > 0) {
        client.publish("R/" + victronCtx.portalID + "/system/0/Serial");
      }
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

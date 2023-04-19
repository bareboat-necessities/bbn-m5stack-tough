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

  typedef enum {
    FLUID_TYPE_NA = -1,
    FUEL = 0,
    FRESH_WATER = 1,
    WASTE_WATER = 2,
    LIVE_WELL = 3,
    LUBRICATION = 4,
    BLACK_WATER = 5
  } fluid_type_e;

  static victron_mqtt_context_t victronCtx;

  bool starts_with(const char* str, const char* pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
  }

  const char* step_into_path(const char* path) {
    if (path == NULL) return NULL;
    const char* s = strchr(path, '/');
    return s != NULL ? s++ : NULL;
  }

  static void victron_mqtt_on_message(String& topic, String& payload) {
    if (topic.endsWith("/system/0/Serial") && victronCtx.needs_id) {
      String str = topic.substring(2);
      str.remove(str.indexOf("/"));
      victronCtx.portalID.clear();
      victronCtx.portalID += str;
      victronCtx.needs_id = false;
    } else {
      const char* lev1 = step_into_path(topic.c_str());
      const char* lev2 = step_into_path(lev1);
      const char* lev3 = step_into_path(lev2);
      const char* lev4 = step_into_path(lev3);
      const char* lev5 = step_into_path(lev4);

      if (lev5 != NULL) {
        if (starts_with(lev3, "tank/")) {
          int num = atoi(lev4);
          if (strcmp(lev5, "FluidType") == 0) {
            // TODO:
          } else if (strcmp(lev5, "Remaining") == 0) {
            // TODO: %
          }
        }
      }

      // topic:    N/0242ac110002/tank/3/FluidType
      // payload:  {"value": 5}

      // topic:    N/0242ac110002/tank/3/Remaining
      // payload:  {"value": 0.18332000076770782}
    }

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
  }

  void victron_mqtt_client_connect(MQTTClient& client) {
    client.onMessage(victron_mqtt_on_message);
    while (!client.connect("arduino" /*clientID*/, "" /*user*/, "" /*password*/)) {
      delay(1000);
    }

    client.subscribe("N/+/+/#");
  }

  void victron_mqtt_client_begin(MQTTClient& client, WiFiClient& net, const char* host, int port) {
    client.setKeepAlive(10);
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

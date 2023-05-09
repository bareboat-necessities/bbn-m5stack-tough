#ifndef NET_MQTT_H
#define NET_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

  /*

  Victron MQQT
  To start simulator wth docker https://github.com/victronenergy/venus-docker
  
  cd venus-docker
  ./run.sh
  
  ./start_services.sh
  ./simulate.sh --with-solarcharger --with-tanks p
  
  To see messages:
  mosquitto_sub -h 192.168.1.64 -p 1883 -t 'N/+/+/#'

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
    } else {
      const char* lev1 = step_into_path(topic.c_str());
      const char* lev2 = step_into_path(lev1);
      const char* lev3 = step_into_path(lev2);
      const char* lev4 = step_into_path(lev3);

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        return;
      }

      if (lev4 != NULL) {
        if (starts_with(lev2, "tank/")) {
          int num = atoi(lev3);
          if (strcmp(lev4, "FluidType") == 0) {
            // topic:    N/0242ac110002/tank/3/FluidType
            // payload:  {"value": 5}
            if (num < MAX_TANKS && num >= 0) {
              int fluid_type_i = doc["value"].as<int>();
              shipDataModel.tanks.tank[num].fluid_type = (fluid_type_e)fluid_type_i;
            }
          } else if (strcmp(lev4, "Remaining") == 0) {
            // topic:    N/0242ac110002/tank/3/Remaining
            // payload:  {"value": 0.18332000076770782}
            if (num < MAX_TANKS && num >= 0) {
              float remaining = doc["value"].as<float>();
              shipDataModel.tanks.tank[num].percent_of_full.pct = remaining * 100;
              shipDataModel.tanks.tank[num].percent_of_full.age = millis();
            }
          }
        } else if (strcmp(lev2, "system/0/Dc/Pv/Power") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.pv.powerW.watt = val;
          shipDataModel.electrical.sys_dc.pv.powerW.age = millis();
        } else if (strcmp(lev2, "system/0/Dc/Pv/Current") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.pv.current.amp = val;
          shipDataModel.electrical.sys_dc.pv.current.age = millis();
        } else if (strcmp(lev2, "system/0/Dc/Battery/Power") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.battery.powerW.watt = val;
          shipDataModel.electrical.sys_dc.battery.powerW.age = millis();
        } else if (strcmp(lev2, "system/0/Dc/Battery/Current") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.battery.current.amp = val;
          shipDataModel.electrical.sys_dc.battery.current.age = millis();
        } else if (strcmp(lev2, "system/0/Dc/Battery/Voltage") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.battery.voltage.volt = val;
          shipDataModel.electrical.sys_dc.battery.voltage.age = millis();
        } else if (strcmp(lev2, "system/0/Dc/Battery/Soc") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_dc.battery.state_of_charge_pct.pct = val;
          shipDataModel.electrical.sys_dc.battery.state_of_charge_pct.age = millis();
        } else if (strcmp(lev2, "system/0/Ac/Consumption/L1/Power") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_ac.consumption[0].powerW.watt = val;
          shipDataModel.electrical.sys_ac.consumption[0].powerW.age = millis();
        } else if (strcmp(lev2, "system/0/Ac/Consumption/L1/Current") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_ac.consumption[0].current.amp = val;
          shipDataModel.electrical.sys_ac.consumption[0].current.age = millis();
        } else if (strcmp(lev2, "system/0/Ac/ActiveIn/L1/Power") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_ac.active_in[0].powerW.watt = val;
          shipDataModel.electrical.sys_ac.active_in[0].powerW.age = millis();
        } else if (strcmp(lev2, "system/0/Ac/ActiveIn/L1/Current") == 0) {
          float val = doc["value"].as<float>();
          shipDataModel.electrical.sys_ac.active_in[0].current.amp = val;
          shipDataModel.electrical.sys_ac.active_in[0].current.age = millis();
        }
      }
    }

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
  }

  void victron_mqtt_client_connect(MQTTClient& client) {
    client.onMessage(victron_mqtt_on_message);
    client.connect("arduino" /*clientID*/, "" /*user*/, "" /*password*/);
    if (client.connected()) {
      client.subscribe("N/+/+/#");
    }
  }

  void victron_mqtt_client_begin(MQTTClient& client, WiFiClient& net, const char* host, int port) {
    client.setKeepAlive(10);
    client.setTimeout(350);
    setKeepAlive(net);
    client.begin(host, port, net);
    victronCtx.lastMillis = millis();
    victron_mqtt_client_connect(client);
  }

  void victron_mqtt_client_loop(MQTTClient& client) {
    client.loop();
    if (!client.connected()) {
      if (millis() - victronCtx.lastMillis > 10000) {
        victronCtx.lastMillis = millis();
        victron_mqtt_client_connect(client);
      }
    }
    if (millis() - victronCtx.lastMillis > 20000) {
      victronCtx.lastMillis = millis();
      // keep alive
      if (victronCtx.portalID.length() > 0 && client.connected()) {
        client.publish("R/" + victronCtx.portalID + "/system/0/Serial");
      }
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

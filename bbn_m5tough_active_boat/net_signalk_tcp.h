#ifndef NET_SIGNALK_TCP_H
#define NET_SIGNALK_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void signalk_greet(WiFiClient& client) {
    String dataFeed = client.readStringUntil('\n');
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
    client.println(data);
    client.flush();
  }

  void setup_signalk_reconnect(NetClient& client, const char* host, int port) {
    app.onRepeat(7000, [&client, host, port]() {
      if (!client.c.connected()) {
        setKeepAlive(client.c);
        if (client.c.connect(host, port, 200)) {
          signalk_greet(client.c);
          app.onDelay(2000, []() {
            getVesselInfo();
          });
        }
      }
      if (client.lastActivity > 0 && (millis() - client.lastActivity) > 10000) {
        client.c.stop();
        client.lastActivity = 0;
      }
    });
  }

  void signalk_subscribe(NetClient& client) {
    app.onAvailable(client.c, [&client]() {
      while (client.c.connected() && client.c.available() > 160 /* Very important for performance and responsiveness */) {
        bool found = signalk_parse(client.c);
        if (found) {
          client.lastActivity = millis();
          break;
        }
      }
    });
  }

  void signalk_begin(NetClient& skClient, const char* host, int port) {
    setKeepAlive(skClient.c);
    setup_signalk_reconnect(skClient, host, port);
    signalk_subscribe(skClient);
    if (skClient.c.connect(host, port, 300)) {
      signalk_greet(skClient.c);
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

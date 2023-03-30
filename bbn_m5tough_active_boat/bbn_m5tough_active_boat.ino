#ifndef NET_SIGNALK_TCP_H
#define NET_SIGNALK_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void signalk_greet(WiFiClient& client) {
    String dataFeed = client.readStringUntil('\n');
    //M5.Lcd.println(dataFeed);
    // esp32 can't handle all volume. request only what is needed for the current screen ???
    // const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"navigation.rateOfTurn\"}]}";
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
    client.println(data);
    client.flush();
  }

  void setup_reconnect(WiFiClient& client, const char* host, int port) {
    app.onRepeat(5000, [&client, host, port]() {
      if (!client.connected()) {
        if (client.connect(host, port)) {
          signalk_greet(client);
        }
      }
    });
  }

  void signalk_subscribe(WiFiClient& client) {

    signalk_greet(client);

    app.onAvailable(client, [&client]() {
      while (client.available() > 256 /* Very important for performance and responsiveness */ && client.connected()) {
        bool found = signalk_parse(client);
        if (found) {
          break;
        }
      }
    });
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

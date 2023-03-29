#ifndef NET_SIGNALK_TCP_H
#define NET_SIGNALK_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void signalk_greet(WiFiClient& client) {
    String dataFeed = client.readStringUntil('\n');
    M5.Lcd.println(dataFeed);
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
    client.println(data);
    client.flush();
  }

  void setup_reconnect(WiFiClient& client, const char* host, int port) {
    app.onRepeat(5000, [&client, host, port]() {
      //M5.Lcd.print("?");
      if (!client.connected()) {
        M5.Lcd.print("*");
        if (client.connect(host, port)) {
          M5.Lcd.print("Reconnected to ");
          M5.Lcd.println(host);
          signalk_greet(client);
        }
      }
    });
  }

  void signalk_subscribe(WiFiClient& client) {

    signalk_greet(client);

    app.onAvailable(client, [&client]() {
      while (client.available() > 0 && client.connected()) {
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

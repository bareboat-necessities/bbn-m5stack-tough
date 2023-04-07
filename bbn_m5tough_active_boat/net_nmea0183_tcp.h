#ifndef NET_NMEA0183_TCP_H
#define NET_NMEA0183_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void setup_nmea0183_reconnect(WiFiClient& client, const char* host, int port) {
    app.onRepeat(5000, [&client, host, port]() {
      if (!client.connected()) {
        setKeepAlive(client);
        if (client.connect(host, port)) {
        }
      }
    });
  }

  void nmea0183_subscribe(WiFiClient& client) {

    app.onAvailable(client, [&client]() {
      while (client.available() > 128 /* Very important for performance and responsiveness */ && client.connected()) {
        bool found = nmea0183_parse(client);
        if (found) {
          break; /* Very important for performance and responsiveness */
        }
      }
    });
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef NET_NMEA0183_TCP_H
#define NET_NMEA0183_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void setup_nmea0183_reconnect(WiFiClient& client, const char* host, int port) {
    app.onRepeat(5000, [&client, host, port]() {
      if (!client.connected()) {
        setKeepAlive(client);
        if (client.connect(host, port, 3000)) {
        }
      }
    });
  }

  void nmea0183_subscribe(WiFiClient& client) {

    app.onAvailable(client, [&client]() {
      while (client.connected() && client.available() > 128 /* Very important for performance and responsiveness */) {
        bool found = nmea0183_parse(client);
        if (found) {
          break; /* Very important for performance and responsiveness */
        }
      }
    });
  }

  void nmea0183_tcp_begin(WiFiClient& nmea0183Client, const char* nmea0183_host, int nmea0183_port) {
    setKeepAlive(nmea0183Client);
    setup_nmea0183_reconnect(nmea0183Client, nmea0183_host, nmea0183_port);
    if (nmea0183Client.connect(nmea0183_host, nmea0183_port)) {
      nmea0183_subscribe(nmea0183Client);
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

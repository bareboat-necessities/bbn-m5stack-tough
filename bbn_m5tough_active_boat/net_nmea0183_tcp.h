#ifndef NET_NMEA0183_TCP_H
#define NET_NMEA0183_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  void setup_nmea0183_reconnect(NetClient& client, const char* host, int port) {
    app.onRepeat(7000, [&client, host, port]() {
      if (!client.c.connected()) {
        setKeepAlive(client.c);
        if (client.c.connect(host, port, 200)) {
        }
      }
      if (client.lastActivity > 0 && (millis() - client.lastActivity) > 10000) {
        client.c.stop();
        client.lastActivity = 0;
      }
    });
  }

  void nmea0183_subscribe(NetClient& client) {
    app.onAvailable(client.c, [&client]() {
      while (client.c.connected() && client.c.available() > 81 /* Very important for performance and responsiveness */) {
        bool found = nmea0183_parse(client.c);
        if (found) {
          client.lastActivity = millis();
          break; /* Very important for performance and responsiveness */
        }
      }
    });
  }

  void nmea0183_tcp_begin(NetClient& nmea0183Client, const char* nmea0183_host, int nmea0183_port) {
    setKeepAlive(nmea0183Client.c);
    setup_nmea0183_reconnect(nmea0183Client, nmea0183_host, nmea0183_port);
    nmea0183_subscribe(nmea0183Client);
    if (nmea0183Client.c.connect(nmea0183_host, nmea0183_port, 300)) {}
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

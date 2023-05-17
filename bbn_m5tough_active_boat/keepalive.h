#ifndef KEEPALIVE_H
#define KEEPALIVE_H

#include <lwip/sockets.h>

#ifdef __cplusplus
extern "C" {
#endif

  static const char *BLANK_IP = "0.0.0.0";

  void setKeepAlive(WiFiClient &wclient) {
    int flags = 1;
    wclient.setSocketOption(SOL_SOCKET, SO_KEEPALIVE, (const void *)&flags, sizeof(flags));
    flags = 10;
    wclient.setSocketOption(IPPROTO_TCP, TCP_KEEPIDLE, (const void *)&flags, sizeof(flags));
    flags = 5;
    wclient.setSocketOption(IPPROTO_TCP, TCP_KEEPCNT, (const void *)&flags, sizeof(flags));
    flags = 5;
    wclient.setSocketOption(IPPROTO_TCP, TCP_KEEPINTVL, (const void *)&flags, sizeof(flags));
  }

  static void disconnect_clients() {
    if (skClient.c.connected()) {
      skClient.c.stop();
    }
    if (pypClient.c.connected()) {
      pypClient.c.stop();
    }
    if (nmea0183Client.c.connected()) {
      nmea0183Client.c.stop();
    }
    if (mqttNetClient.connected()) {
      mqttNetClient.stop();
    }
  }

  static void ESP_restart() {
    disconnect_clients();
    ESP.restart();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

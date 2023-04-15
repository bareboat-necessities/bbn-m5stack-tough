#ifndef KEEPALIVE_H
#define KEEPALIVE_H

#include <lwip/sockets.h>

#ifdef __cplusplus
extern "C" {
#endif

  void setKeepAlive(Client &client) {
    int flags = 1;
    client.setSocketOption(SOL_SOCKET, SO_KEEPALIVE, (const void *)&flags, sizeof(flags));
    flags = 10;
    client.setSocketOption(IPPROTO_TCP, TCP_KEEPIDLE, (const void *)&flags, sizeof(flags));
    flags = 5;
    client.setSocketOption(IPPROTO_TCP, TCP_KEEPCNT, (const void *)&flags, sizeof(flags));
    flags = 5;
    client.setSocketOption(IPPROTO_TCP, TCP_KEEPINTVL, (const void *)&flags, sizeof(flags));
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef NMEA0183_PARSE_H
#define NMEA0183_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  bool nmea0183_parse(WiFiClient& client) {
    bool found = false;
    String dataFeed = client.readStringUntil('\n');
    return found;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif



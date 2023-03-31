#ifndef PYPILOT_PARSE_H
#define PYPILOT_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  bool pypilot_parse(WiFiClient& client) {
    bool found = false;
    String dataFeed = client.readStringUntil('\n');
    // ap.heading=164.798
    // ap.heading_command=220.0000
    // ap.enabled=false
    // ap.mode="compass"
    if (dataFeed.length() > 0) {
      if (dataFeed.startsWith("ap.heading=")) {

      } else if (dataFeed.startsWith("ap.heading_command=") {

      } else if (dataFeed.startsWith("ap.enabled=\"") {

      } else if (dataFeed.startsWith("ap.mode=\"") {

      }
      return found;
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif


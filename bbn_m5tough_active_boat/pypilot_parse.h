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
        // TODO:
      } else if (dataFeed.startsWith("ap.heading_command=")) {
        // TODO:
      } else if (dataFeed.startsWith("ap.enabled=true")) {
        shipDataModel.steering.autopilot.ap_state.st = ap_state_e::ENGAGED;
        shipDataModel.steering.autopilot.ap_state.age = millis();
      } else if (dataFeed.startsWith("ap.enabled=false")) {
        shipDataModel.steering.autopilot.ap_state.st = ap_state_e::STANDBY;
        shipDataModel.steering.autopilot.ap_state.age = millis();
      } else if (dataFeed.startsWith("ap.mode=\"")) {
        // TODO:
      }
      return found;
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif


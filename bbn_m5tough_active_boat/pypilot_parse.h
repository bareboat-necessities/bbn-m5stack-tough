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
      found = true;
      if (dataFeed.startsWith("ap.heading=")) {
        shipDataModel.steering.autopilot.heading.deg =
          strtof(dataFeed.substring(strlen("ap.heading="), dataFeed.length()).c_str(), NULL);
        shipDataModel.steering.autopilot.heading.age = millis();
      } else if (dataFeed.startsWith("ap.heading_command=")) {
        shipDataModel.steering.autopilot.command.deg =
          strtof(dataFeed.substring(strlen("ap.heading_command="), dataFeed.length()).c_str(), NULL);
        shipDataModel.steering.autopilot.command.age = millis();
      } else if (dataFeed.startsWith("ap.enabled=true")) {
        shipDataModel.steering.autopilot.ap_state.st = ap_state_e::ENGAGED;
        shipDataModel.steering.autopilot.ap_state.age = millis();
      } else if (dataFeed.startsWith("ap.enabled=false")) {
        shipDataModel.steering.autopilot.ap_state.st = ap_state_e::STANDBY;
        shipDataModel.steering.autopilot.ap_state.age = millis();
      } else if (dataFeed.startsWith("ap.mode=\"")) {
        String mode = dataFeed.substring(strlen("ap.mode=\""), dataFeed.length() - 1);
        shipDataModel.steering.autopilot.ap_mode.mode = ap_mode_e::MODE_NA;
        shipDataModel.steering.autopilot.ap_mode.age = millis();
        if (mode == "gps") {
          shipDataModel.steering.autopilot.ap_mode.mode = ap_mode_e::COG_TRUE;
        } else if (mode == "wind") {
          shipDataModel.steering.autopilot.ap_mode.mode = ap_mode_e::APP_WIND;
        } else if (mode == "compass") {
          shipDataModel.steering.autopilot.ap_mode.mode = ap_mode_e::HEADING_MAG;
        } else if (mode == "true wind") {
          shipDataModel.steering.autopilot.ap_mode.mode = ap_mode_e::TRUE_WIND;
        }
      } else if (dataFeed.startsWith("servo.voltage=")) {
        shipDataModel.steering.autopilot.ap_servo.voltage.volt =
          strtof(dataFeed.substring(strlen("servo.voltage="), dataFeed.length()).c_str(), NULL);
        shipDataModel.steering.autopilot.ap_servo.voltage.age = millis();
      } else if (dataFeed.startsWith("servo.amp_hours=")) {
        shipDataModel.steering.autopilot.ap_servo.amp_hr.amp_hr =
          strtof(dataFeed.substring(strlen("servo.amp_hours="), dataFeed.length()).c_str(), NULL);
        shipDataModel.steering.autopilot.ap_servo.amp_hr.age = millis();
      } else if (dataFeed.startsWith("servo.controller_temp=")) {
        shipDataModel.steering.autopilot.ap_servo.controller_temp.deg_C =
          strtof(dataFeed.substring(strlen("servo.controller_temp="), dataFeed.length()).c_str(), NULL);
        shipDataModel.steering.autopilot.ap_servo.controller_temp.age = millis();
      } else {
        found = false;
      }      
    }
    return found;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

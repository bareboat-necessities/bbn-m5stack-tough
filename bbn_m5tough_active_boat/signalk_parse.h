#ifndef SIGNALK_PARSE_H
#define SIGNALK_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  bool signalk_parse(Stream& stream) {
    bool found = false;
    DynamicJsonDocument doc(4096);
    DeserializationError err = deserializeJson(doc, stream);
    // Parse succeeded?
    if (err) {
      return false;
    }
    JsonObject obj = doc.as<JsonObject>();
    if (obj != NULL) {
      auto update_value = [&](String& path, size_t& u_idx, size_t& v_idx, JsonVariant& value) {
        if (path == "navigation.rateOfTurn") {
          if (value.is<float>()) {
            shipDataModel.navigation.rate_of_turn.deg_min = 60 * value.as<float>() * 180 / PI;
            shipDataModel.navigation.rate_of_turn.age = millis();
          }
        } else if (path == "steering.rudderAngle") {
          if (value.is<float>()) {
            shipDataModel.steering.rudder_angle.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.steering.rudder_angle.age = millis();
          }
        } else if (path == "environment.depth.belowKeel") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_keel.m = value.as<float>();
            shipDataModel.environment.depth.below_keel.age = millis();
          }
        } else if (path == "environment.depth.belowTransducer") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_transducer.m = value.as<float>();
            shipDataModel.environment.depth.below_transducer.age = millis();
          }
        } else if (path == "environment.depth.belowSurface") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_surface.m = value.as<float>();
            shipDataModel.environment.depth.below_surface.age = millis();
          }
        } else if (path == "environment.wind.angleApparent") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.apparent_wind_angle.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.environment.wind.apparent_wind_angle.age = millis();
          }
        } else if (path == "environment.wind.angleTrueGround") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.ground_wind_angle.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.environment.wind.ground_wind_angle.age = millis();
          }
        } else if (path == "environment.wind.angleTrueWater") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.true_wind_angle.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.environment.wind.true_wind_angle.age = millis();
          }
        } else if (path == "environment.wind.speedApparent") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
        } else if (path == "environment.wind.speedOverGround") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.ground_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.ground_wind_speed.age = millis();
          }
        } else if (path == "environment.wind.speedTrue") {
          if (value.is<float>()) {
            shipDataModel.environment.wind.true_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
        } else if (path == "navigation.speedOverGround") {
          if (value.is<float>()) {
            shipDataModel.navigation.speed_over_ground.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.navigation.speed_over_ground.age = millis();
          }
        } else if (path == "navigation.speedThroughWater") {
          if (value.is<float>()) {
            shipDataModel.navigation.speed_through_water.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.navigation.speed_through_water.age = millis();
          }
        } else if (path == "navigation.headingMagnetic") {
          if (value.is<float>()) {
            shipDataModel.navigation.heading_mag.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.navigation.heading_mag.age = millis();
          }
        } else if (path == "navigation.courseOverGroundTrue") {
          if (value.is<float>()) {
            shipDataModel.navigation.course_over_ground_true.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.navigation.course_over_ground_true.age = millis();
          }
        } else if (path == "environment.outside.pressure") {
          if (value.is<float>()) {
            shipDataModel.environment.air_outside.pressure.hPa = value.as<float>() / 100.0;
            shipDataModel.environment.air_outside.pressure.age = millis();
          }
        } else if (path == "environment.outside.humidity") {
          if (value.is<float>()) {
            shipDataModel.environment.air_outside.humidity_pct.pct = value.as<float>() * 100.0;
            shipDataModel.environment.air_outside.humidity_pct.age = millis();
          }
        } else if (path == "environment.outside.temperature") {
          if (value.is<float>()) {
            shipDataModel.environment.air_outside.temp_deg_C.deg_C = value.as<float>() - 273.15;
            shipDataModel.environment.air_outside.temp_deg_C.age = millis();
          }
        } else if (path == "environment.outside.illuminance") {
          if (value.is<float>()) {
            shipDataModel.environment.air_outside.illuminance.lux = value.as<float>();
            shipDataModel.environment.air_outside.illuminance.age = millis();
          }
        } else if (path == "navigation.courseRhumbline.crossTrackError") {
          if (value.is<float>()) {
            shipDataModel.navigation.course_rhumbline.cross_track_error.m = value.as<float>();
            shipDataModel.navigation.course_rhumbline.cross_track_error.age = millis();
          }
        } else if (path == "navigation.courseRhumbline.bearingTrackTrue") {
          if (value.is<float>()) {
            shipDataModel.navigation.course_rhumbline.bearing_track_true.deg = value.as<float>() * 180.0 / PI;
            shipDataModel.navigation.course_rhumbline.bearing_track_true.age = millis();
          }
        } else if (path == "navigation.courseRhumbline.nextPoint.distance") {
          if (value.is<float>()) {
            shipDataModel.navigation.course_rhumbline.next_point.distance.m = value.as<float>();
            shipDataModel.navigation.course_rhumbline.next_point.distance.age = millis();
          }
        } else if (path == "navigation.courseRhumbline.nextPoint.velocityMadeGood") {
          if (value.is<float>()) {
            shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
            shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.age = millis();
          }
        } else if (path == "navigation.state") {
          if (value.is<String>()) {
            String val = value.as<String>();
            if (val != NULL) {
              if (val == "moored") {
                shipDataModel.navigation.state.st = nav_state_e::NS_MOORED;
                shipDataModel.navigation.state.age = millis();
              } else if (val == "sailing") {
                shipDataModel.navigation.state.st = nav_state_e::NS_SAILING;
                shipDataModel.navigation.state.age = millis();
              } else if (val == "motoring") {
                shipDataModel.navigation.state.st = nav_state_e::NS_MOTORING;
                shipDataModel.navigation.state.age = millis();
              } else if (val == "anchored") {
                shipDataModel.navigation.state.st = nav_state_e::NS_ANCHORED;
                shipDataModel.navigation.state.age = millis();
              }
            }
          }
        } else if (path == "navigation.position") {
          if (value.containsKey("longitude") && value.containsKey("latitude")) {
            if (value["longitude"].is<float>() && value["latitude"].is<float>()) {
              shipDataModel.navigation.position.lat.deg = value["latitude"].as<float>();  // SignalK sends degrees for it
              shipDataModel.navigation.position.lat.age = millis();
              shipDataModel.navigation.position.lon.deg = value["longitude"].as<float>(); // SignalK sends degrees for it
              shipDataModel.navigation.position.lon.age = millis();
            }
          }
        } else if (path != NULL && path.startsWith("propulsion.")) {
          String engineID = path.substring(11);
          int idx = engineID.indexOf('.');
          if (idx > 0) {
            engineID = engineID.substring(0, idx);
            if (engineID != NULL) {
              engine_t *eng = lookup_engine(engineID.c_str());
              if (eng != NULL) {                
                if (path == (String("propulsion.") + engineID + ".revolutions")) {
                  if (value.is<float>()) {
                    eng->revolutions_RPM.rpm = value.as<float>() * 60;
                    eng->revolutions_RPM.age = millis();
                  }
                } else if (path == (String("propulsion.") + engineID + ".temperature")) {
                  if (value.is<float>()) {
                    eng->temp_deg_C.deg_C = value.as<float>() - 273.15;
                    eng->temp_deg_C.age = millis();
                  }
                } else if (path == (String("propulsion.") + engineID + ".oilPressure")) {
                  if (value.is<float>()) {
                    eng->oil_pressure.hPa = value.as<float>() / 100.0;
                    eng->oil_pressure.age = millis();
                  }
                } else if (path == (String("propulsion.") + engineID + ".alternatorVoltage")) {
                  if (value.is<float>()) {
                    eng->alternator_voltage.volt = value.as<float>();
                    eng->alternator_voltage.age = millis();
                  }
                }
              }
            }
          }
        }
      };

      JsonArray updates = obj["updates"];
      if (updates != NULL) {
        for (size_t i_u = 0; i_u < updates.size(); i_u++) {
          JsonObject update = updates[i_u];
          if (update != NULL) {
            JsonArray values = update["values"];
            if (values != NULL) {
              for (size_t i_v = 0; i_v < values.size(); i_v++) {
                JsonObject valueObj = values[i_v];
                if (valueObj != NULL) {
                  if (valueObj.containsKey("path")) {
                    String path = valueObj["path"].as<const char*>();
                    if (path != NULL) {
                      JsonVariant value = valueObj["value"];
                      if (value != NULL) {
                        update_value(path, i_u, i_v, value);
                        found = true;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    return found;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

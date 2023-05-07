#ifndef SIGNALK_PARSE_H
#define SIGNALK_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  void set_vessel_nav_state(String val) {
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
        const char* p = path.c_str();
        if (starts_with(p, "navigation.")) {
          const char* t = step_into_token(p);
          if (strcmp(t, "rateOfTurn") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.rate_of_turn.deg_min = 60 * value.as<float>() * 180 / PI;
              shipDataModel.navigation.rate_of_turn.age = millis();
            }
          } else if (strcmp(t, "headingMagnetic") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.heading_mag.deg = value.as<float>() * 180.0 / PI;
              shipDataModel.navigation.heading_mag.age = millis();
            }
          } else if (strcmp(t, "position") == 0) {
            if (value.containsKey("longitude") && value.containsKey("latitude")) {
              if (value["longitude"].is<float>() && value["latitude"].is<float>()) {
                shipDataModel.navigation.position.lat.deg = value["latitude"].as<float>();  // SignalK sends degrees for it
                shipDataModel.navigation.position.lat.age = millis();
                shipDataModel.navigation.position.lon.deg = value["longitude"].as<float>();  // SignalK sends degrees for it
                shipDataModel.navigation.position.lon.age = millis();
              }
            }
          } else if (strcmp(t, "speedOverGround") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.speed_over_ground.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
              shipDataModel.navigation.speed_over_ground.age = millis();
            }
          } else if (strcmp(t, "speedThroughWater") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.speed_through_water.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
              shipDataModel.navigation.speed_through_water.age = millis();
            }
          } else if (strcmp(t, "courseOverGroundTrue") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.course_over_ground_true.deg = value.as<float>() * 180.0 / PI;
              shipDataModel.navigation.course_over_ground_true.age = millis();
            }
          } else if (strcmp(t, "courseRhumbline.crossTrackError") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.course_rhumbline.cross_track_error.m = value.as<float>();
              shipDataModel.navigation.course_rhumbline.cross_track_error.age = millis();
            }
          } else if (strcmp(t, "courseRhumbline.bearingTrackTrue") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.course_rhumbline.bearing_track_true.deg = value.as<float>() * 180.0 / PI;
              shipDataModel.navigation.course_rhumbline.bearing_track_true.age = millis();
            }
          } else if (strcmp(t, "courseRhumbline.nextPoint.distance") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.course_rhumbline.next_point.distance.m = value.as<float>();
              shipDataModel.navigation.course_rhumbline.next_point.distance.age = millis();
            }
          } else if (strcmp(t, "courseRhumbline.nextPoint.velocityMadeGood") == 0) {
            if (value.is<float>()) {
              shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
              shipDataModel.navigation.course_rhumbline.next_point.velocity_made_good.age = millis();
            }
          } else if (strcmp(t, "state") == 0) {
            if (value.is<String>()) {
              String val = value.as<String>();
              if (val != NULL) {
                set_vessel_nav_state(val);
              }
            }
          }
        } else if (starts_with(p, "environment.")) {
          const char* t = step_into_token(p);
          if (starts_with(t, "wind.")) {
            const char* w = step_into_token(t);
            if (strcmp(w, "angleApparent") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.apparent_wind_angle.deg = value.as<float>() * 180.0 / PI;
                shipDataModel.environment.wind.apparent_wind_angle.age = millis();
              }
            } else if (strcmp(w, "angleTrueGround") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.ground_wind_angle.deg = value.as<float>() * 180.0 / PI;
                shipDataModel.environment.wind.ground_wind_angle.age = millis();
              }
            } else if (strcmp(w, "angleTrueWater") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.true_wind_angle.deg = value.as<float>() * 180.0 / PI;
                shipDataModel.environment.wind.true_wind_angle.age = millis();
              }
            } else if (strcmp(w, "speedApparent") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.apparent_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
                shipDataModel.environment.wind.apparent_wind_speed.age = millis();
              }
            } else if (strcmp(w, "speedOverGround") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.ground_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
                shipDataModel.environment.wind.ground_wind_speed.age = millis();
              }
            } else if (strcmp(w, "speedTrue") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.wind.true_wind_speed.kn = value.as<float>() / _GPS_MPS_PER_KNOT;
                shipDataModel.environment.wind.true_wind_speed.age = millis();
              }
            }
          } else if (starts_with(t, "depth.")) {
            const char* d = step_into_token(t);
            if (strcmp(d, "belowKeel") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.depth.below_keel.m = value.as<float>();
                shipDataModel.environment.depth.below_keel.age = millis();
              }
            } else if (strcmp(d, "belowTransducer") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.depth.below_transducer.m = value.as<float>();
                shipDataModel.environment.depth.below_transducer.age = millis();
              }
            } else if (strcmp(d, "belowSurface") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.depth.below_surface.m = value.as<float>();
                shipDataModel.environment.depth.below_surface.age = millis();
              }
            }
          } else if (starts_with(t, "outside.")) {
            const char* o = step_into_token(t);
            if (strcmp(o, "pressure") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.air_outside.pressure.hPa = value.as<float>() / 100.0;
                shipDataModel.environment.air_outside.pressure.age = millis();
              }
            } else if (strcmp(o, "humidity") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.air_outside.humidity_pct.pct = value.as<float>() * 100.0;
                shipDataModel.environment.air_outside.humidity_pct.age = millis();
              }
            } else if (strcmp(o, "temperature") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.air_outside.temp_deg_C.deg_C = value.as<float>() - 273.15;
                shipDataModel.environment.air_outside.temp_deg_C.age = millis();
              }
            } else if (strcmp(o, "illuminance") == 0) {
              if (value.is<float>()) {
                shipDataModel.environment.air_outside.illuminance.lux = value.as<float>();
                shipDataModel.environment.air_outside.illuminance.age = millis();
              }
            }
          }
        } else if (starts_with(p, "steering.")) {
          const char* t = step_into_token(p);
          if (strcmp(t, "rudderAngle") == 0) {
            if (value.is<float>()) {
              shipDataModel.steering.rudder_angle.deg = value.as<float>() * 180.0 / PI;
              shipDataModel.steering.rudder_angle.age = millis();
            }
          }
        } else if (starts_with(p, "propulsion.")) {
          String engineID = path.substring(11);
          int idx = engineID.indexOf('.');
          if (idx > 0) {
            engineID = engineID.substring(0, idx);
            if (engineID != NULL) {
              engine_t* eng = lookup_engine(engineID.c_str());
              if (eng != NULL) {
                String prefix = String("propulsion.") + engineID;
                if (path == (prefix + ".revolutions")) {
                  if (value.is<float>()) {
                    eng->revolutions_RPM.rpm = value.as<float>() * 60;
                    eng->revolutions_RPM.age = millis();
                  }
                } else if (path == (prefix + ".temperature")) {
                  if (value.is<float>()) {
                    eng->temp_deg_C.deg_C = value.as<float>() - 273.15;
                    eng->temp_deg_C.age = millis();
                  }
                } else if (path == (prefix + ".oilPressure")) {
                  if (value.is<float>()) {
                    eng->oil_pressure.hPa = value.as<float>() / 100.0;
                    eng->oil_pressure.age = millis();
                  }
                } else if (path == (prefix + ".alternatorVoltage")) {
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

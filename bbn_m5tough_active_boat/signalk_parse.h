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
        } else if (path == "navigation.speedThroughWaterLongitudinal") {
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
        } else if (path == "navigation.position") {
          if (value.containsKey("longitude") && value.containsKey("latitude")) {
            if (value["longitude"].is<float>() && value["latitude"].is<float>()) {
              shipDataModel.navigation.position.lat.deg = value["latitude"].as<float>();  // SignalK sends degrees for it
              shipDataModel.navigation.position.lat.age = millis();
              shipDataModel.navigation.position.lon.deg = value["longitude"].as<float>(); // SignalK sends degrees for it
              shipDataModel.navigation.position.lon.age = millis();
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

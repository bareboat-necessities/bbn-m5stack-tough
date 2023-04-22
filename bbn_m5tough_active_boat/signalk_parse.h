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
        //String updatedValue = path;
        if (path == "navigation.rateOfTurn") {
          if (value.is<float>()) {
            shipDataModel.navigation.rate_of_turn.deg_min = 60 * value.as<float>() * 180 / PI;
            shipDataModel.navigation.rate_of_turn.age = millis();
          }
        } else if (path == "environment.depth.belowKeel") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_keel = value.as<float>();
            shipDataModel.environment.depth.age = millis();
          }
        } else if (path == "environment.depth.belowTransducer") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_transducer = value.as<float>();
            shipDataModel.environment.depth.age = millis();
          }
        } else if (path == "environment.depth.belowSurface") {
          if (value.is<float>()) {
            shipDataModel.environment.depth.below_surface = value.as<float>();
            shipDataModel.environment.depth.age = millis();
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
        }
        /*
        if (value.is<float>()) {
          updatedValue = updatedValue + " float: ";
        } else if (value.is<int>()) {
          updatedValue = updatedValue + " int: ";
        } else if (value.is<const char*>()) {
          updatedValue = updatedValue + " str: ";
        } else if (value.is<boolean>()) {
          updatedValue = updatedValue + " bool: ";
        } else {
          updatedValue = updatedValue + " ";
        }
        serializeJson(value, updatedValue);
        */
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

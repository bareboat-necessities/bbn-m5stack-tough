#ifndef SIGNALK_TCP_H
#define SIGNALK_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

  bool signalk_parse(Stream& stream) {
    bool found = false;
    DynamicJsonDocument doc(4096);
    DeserializationError err = deserializeJson(doc, stream);
    // Parse succeeded?
    if (err) {
      //Serial.print(F("deserializeJson() returned "));
      //Serial.println(err.c_str());
      M5.Lcd.println(F("deserializeJson() returned "));
      M5.Lcd.println(err.c_str());
      return false;
    }
    JsonObject obj = doc.as<JsonObject>();
    if (obj != NULL) {
      auto update_value = [&](String& path, size_t& u_idx, size_t& v_idx, JsonVariant& value) {
        String updatedValue = path;
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
        //M5.Lcd.println(updatedValue);
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

  int samples = 20;

  void signalk_greet(WiFiClient& client) {
    String dataFeed = client.readStringUntil('\n');
    M5.Lcd.println(dataFeed);
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\"}]}";
    client.println(data);
    client.flush();
  }

  void signalk_subscribe(WiFiClient& client) {

    signalk_greet(client);

    app.onAvailable(client, [samples, &client]() {    
      while (client.available() > 128 /* Very important for performance and responsiveness */ && client.connected()) {
        bool found = signalk_parse(client);
        if (found) {
          //samples--;
          if (samples <= 0) {
            //client.stop();
          }
          break;
        }
      }
      if (!client.connected()) {
        //M5.Lcd.println("Connection lost!");
      }
    });
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

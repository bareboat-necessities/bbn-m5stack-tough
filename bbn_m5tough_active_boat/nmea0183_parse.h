#ifndef NMEA0183_PARSE_H
#define NMEA0183_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  // Sample: $WIMWV,27,R,00,N,A*26
  const char* wind_prefix[] = { "WIMWV", "IIMWV" };

  int wind_prefix_index = 0;

  TinyGPSCustom windAngle(gps, wind_prefix[0], 1);      // Example: 214.8
  TinyGPSCustom windReference(gps, wind_prefix[0], 2);  // Reference: R = Relative, T = True
  TinyGPSCustom windSpeed(gps, wind_prefix[0], 3);      // Example: 0.1
  TinyGPSCustom windSpeedUnit(gps, wind_prefix[0], 4);  // Units: M = Meter per second, N = Knots, K = Kilometres per hour

  TinyGPSCustom windAngleI(gps, wind_prefix[1], 1);      // Example: 214.8
  TinyGPSCustom windReferenceI(gps, wind_prefix[1], 2);  // Reference: R = Relative, T = True
  TinyGPSCustom windSpeedI(gps, wind_prefix[1], 3);      // Example: 0.1
  TinyGPSCustom windSpeedUnitI(gps, wind_prefix[1], 4);  // Units: M = Meter per second, N = Knots, K = Kilometres per hour

  const char* heading_true_prefix = "IIHDT";  // Sample: $IIHDT,347.4,T*26
  const char* heading_mag_prefix = "APHDM";   // Sample: $APHDM,0.000,M*33

  TinyGPSCustom headingTrue(gps, heading_true_prefix, 1);  // Example: 347.4
  TinyGPSCustom headingMag(gps, heading_mag_prefix, 1);    // Example: 347.4

  // Sample: $APXDR,A,0.000,D,PTCH*7B
  // Sample: $APXDR,A,0.000,D,ROLL*69
  const char* custom_data_prefix = "APXDR";

  TinyGPSCustom customData_Value(gps, custom_data_prefix, 2);
  TinyGPSCustom customData_Type(gps, custom_data_prefix, 4);

  void parse_sentence(const char* line) {
    for (; *line != '\0'; line++) {
      gps.encode(*line);
    }
  }

  bool nmea_parse(String& line) {
    parse_sentence(line.c_str());
    return line.length() > 0;
  }

  float parse_float(const char* str) {
    return strtof(str, NULL);
  }

#define NMEA_END_CHAR_1 '\n'
#define NMEA_MAX_LENGTH 128

  uint8_t nmea_get_checksum(const char* sentence) {
    const char* n = sentence + 1;  // Plus one, skip '$'
    uint8_t chk = 0;
    /* While current char isn't '*' or sentence ending (newline) */
    while ('*' != *n && NMEA_END_CHAR_1 != *n) {
      if ('\0' == *n || n - sentence > NMEA_MAX_LENGTH) {
        /* Sentence too long or short */
        return 0;
      }
      chk ^= (uint8_t)*n;
      n++;
    }
    return chk;
  }

  tm gps_time_to_tm(TinyGPSDate& d, TinyGPSTime& t) {
    tm result;
    result.tm_sec = t.second();
    result.tm_min = t.minute();
    result.tm_hour = t.hour();

    result.tm_mday = d.day();
    result.tm_mon = d.month() - 1;
    result.tm_year = d.year() - 1900;
    return result;
  }

  bool nmea0183_parse(WiFiClient& client) {
    bool found = false;
    String dataLine = client.readStringUntil('\n');
    if (dataLine.length() > 0 && dataLine.charAt(0) == '!') {
      // TODO: AIS
      return true;
    } else {
      found = nmea_parse(dataLine);
    }

    if (customData_Value.isUpdated() && customData_Type.isValid()) {
      if (strcmp("PTCH", customData_Type.value()) == 0 || strcmp("PITCH", customData_Type.value()) == 0) {
        shipDataModel.navigation.attitude.pitch.deg = parse_float(customData_Value.value());
        shipDataModel.navigation.attitude.pitch.age = millis();
      } else if (strcmp("ROLL", customData_Type.value()) == 0) {
        shipDataModel.navigation.attitude.heel.deg = parse_float(customData_Value.value());
        shipDataModel.navigation.attitude.heel.age = millis();
      }
    }
    if (gps.location.isUpdated()) {
      shipDataModel.navigation.position.lat.deg = gps.location.lat();
      shipDataModel.navigation.position.lon.deg = gps.location.lng();
      shipDataModel.navigation.position.lat.age = millis();
      shipDataModel.navigation.position.lon.age = millis();
    }
    if (gps.altitude.isUpdated()) {
      // TODO:
    }
    if (gps.speed.isUpdated()) {
      shipDataModel.navigation.speed_over_ground.kn = gps.speed.knots();
      shipDataModel.navigation.speed_over_ground.age = millis();
    }
    if (gps.course.isUpdated()) {
      shipDataModel.navigation.course_over_ground_true.deg = gps.course.deg();
      shipDataModel.navigation.course_over_ground_true.age = millis();
    }
    if (gps.time.isUpdated()) {
      tm new_time = gps_time_to_tm(gps.date, gps.time);
      //if (difftime(mktime(&new_time), mktime(&(shipDataModel.environment.time_gps.t))) > 0) {
      shipDataModel.environment.time_gps.t = new_time;
      shipDataModel.environment.time_gps.age = millis();
      //}
    }
    if (headingTrue.isUpdated() && headingTrue.isValid()) {
      shipDataModel.navigation.heading_true.deg = norm_deg(parse_float(headingTrue.value()));
      shipDataModel.navigation.heading_true.age = millis();
    }
    if (headingMag.isUpdated()) {
      shipDataModel.navigation.heading_mag.deg = norm_deg(parse_float(headingMag.value()));
      shipDataModel.navigation.heading_mag.age = millis();
    }
    if (windAngle.isUpdated() && windReference.isUpdated()) {
      if (strcmp("R", windReference.value()) == 0) {
        shipDataModel.environment.wind.apparent_wind_angle.deg = norm180_deg(parse_float(windAngle.value()));
        shipDataModel.environment.wind.apparent_wind_angle.age = millis();
      }
      if (strcmp("T", windReference.value()) == 0) {
        shipDataModel.environment.wind.true_wind_angle.deg = norm180_deg(parse_float(windAngle.value()));
        shipDataModel.environment.wind.true_wind_angle.age = millis();
      }
    }
    if (windAngleI.isUpdated() && windReferenceI.isUpdated()) {
      if (strcmp("R", windReferenceI.value()) == 0) {
        shipDataModel.environment.wind.apparent_wind_angle.deg = norm180_deg(parse_float(windAngleI.value()));
        shipDataModel.environment.wind.apparent_wind_angle.age = millis();
      }
      if (strcmp("T", windReferenceI.value()) == 0) {
        shipDataModel.environment.wind.true_wind_angle.deg = norm180_deg(parse_float(windAngleI.value()));
        shipDataModel.environment.wind.true_wind_angle.age = millis();
      }
      if (windSpeed.isUpdated() && windSpeedUnit.isUpdated()) {
        if (strcmp("R", windReference.value()) == 0) {
          if (strcmp("N", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeed.value());
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
          if (strcmp("M", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeed.value()) / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
          if (strcmp("K", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeed.value()) / _GPS_KMPH_PER_KNOT;
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
        }
        if (strcmp("T", windReference.value()) == 0) {
          if (strcmp("N", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeed.value());
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
          if (strcmp("M", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeed.value()) / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
          if (strcmp("K", windSpeedUnit.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeed.value()) / _GPS_KMPH_PER_KNOT;
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
        }
      }
      if (windSpeedI.isUpdated() && windSpeedUnitI.isUpdated()) {
        if (strcmp("R", windReferenceI.value()) == 0) {
          if (strcmp("N", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeedI.value());
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
          if (strcmp("M", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeedI.value()) / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
          if (strcmp("K", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.apparent_wind_speed.kn = parse_float(windSpeedI.value()) / _GPS_KMPH_PER_KNOT;
            shipDataModel.environment.wind.apparent_wind_speed.age = millis();
          }
        }
        if (strcmp("T", windReferenceI.value()) == 0) {
          if (strcmp("N", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeedI.value());
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
          if (strcmp("M", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeedI.value()) / _GPS_MPS_PER_KNOT;
            shipDataModel.environment.wind.true_wind_speed.age = millis();
          }
          if (strcmp("K", windSpeedUnitI.value()) == 0) {
            shipDataModel.environment.wind.true_wind_speed.kn = parse_float(windSpeedI.value()) / _GPS_KMPH_PER_KNOT;
            shipDataModel.environment.wind.true_wind_speed.age = millis();
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

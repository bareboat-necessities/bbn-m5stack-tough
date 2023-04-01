#ifndef NMEA0183_PARSE_H
#define NMEA0183_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

  TinyGPSPlus gps;

  // Sample: $WIMWV,27,R,00,N,A*26
  char* wind_prefix[] = { "WIMWV", "IIMWV" };

  int wind_prefix_index = 0;

  TinyGPSCustom windAngle(gps, wind_prefix[0], 1);      // Example: 214.8
  TinyGPSCustom windReference(gps, wind_prefix[0], 2);  // Reference: R = Relative, T = True
  TinyGPSCustom windSpeed(gps, wind_prefix[0], 3);      // Example: 0.1
  TinyGPSCustom windSpeedUnit(gps, wind_prefix[0], 4);  // Units: M = Meter per second, N = Knots, K = Kilometres per hour

  TinyGPSCustom windAngleI(gps, wind_prefix[1], 1);      // Example: 214.8
  TinyGPSCustom windReferenceI(gps, wind_prefix[1], 2);  // Reference: R = Relative, T = True
  TinyGPSCustom windSpeedI(gps, wind_prefix[1], 3);      // Example: 0.1
  TinyGPSCustom windSpeedUnitI(gps, wind_prefix[1], 4);  // Units: M = Meter per second, N = Knots, K = Kilometres per hour

  char* heading_true_prefix = "IIHDT";  // Sample: $IIHDT,347.4,T*26
  char* heading_mag_prefix = "APHDM";   // Sample: $APHDM,0.000,M*33

  TinyGPSCustom headingTrue(gps, heading_true_prefix, 1);  // Example: 347.4
  TinyGPSCustom headingMag(gps, heading_mag_prefix, 1);    // Example: 347.4

  // Sample: $APXDR,A,0.000,D,PTCH*7B
  // Sample: $APXDR,A,0.000,D,ROLL*69
  char* custom_data_prefix = "APXDR";

  TinyGPSCustom customData_Value(gps, custom_data_prefix, 2);
  TinyGPSCustom customData_Type(gps, custom_data_prefix, 4);

  void parse_sentence(const char* line) {
    for (; *line != '\0'; line++) {
      gps.encode(*line);
    }
  }

  bool nmea_parse(String& line) {
    //M5.Lcd.println(line);
    parse_sentence(line.c_str());
    return line.length() > 0;
  }

  float parse_float(const char* str) {
    return strtof(str, NULL);
  }

  void displayGPSData() {
    if (gps.location.isUpdated()) {
      M5.Lcd.print("POS:  ");
      M5.Lcd.print(gps.location.lat());
      M5.Lcd.print(" ");
      M5.Lcd.println(gps.location.lng());
    }
    if (gps.altitude.isUpdated()) {
      M5.Lcd.print("ALT:  ");
      M5.Lcd.println(gps.altitude.feet());
    }
    if (gps.speed.isUpdated()) {
      M5.Lcd.print("SOG:  ");
      M5.Lcd.println(gps.speed.knots());
    }
    if (gps.course.isUpdated()) {
      M5.Lcd.print("COG:  ");
      M5.Lcd.println(gps.course.deg());
    }
    if (gps.time.isUpdated()) {
      M5.Lcd.print("TIME: ");
      M5.Lcd.print(gps.time.hour());
      M5.Lcd.print(":");
      M5.Lcd.print(gps.time.minute());
      M5.Lcd.print(":");
      M5.Lcd.println(gps.time.second());
    }
    if (headingTrue.isUpdated()) {
      M5.Lcd.print("HDT:  ");
      M5.Lcd.println(headingTrue.value());
    }
    if (headingMag.isUpdated()) {
      M5.Lcd.print("HDM:  ");
      M5.Lcd.println(headingMag.value());
    }
    if (customData_Value.isUpdated() && customData_Type.isValid()) {
      M5.Lcd.print(customData_Type.value());
      M5.Lcd.print(": ");
      M5.Lcd.println(customData_Value.value());
    }
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

  bool nmea0183_parse(WiFiClient& client) {
    bool found = false;
    String dataLine = client.readStringUntil('\n');
    found = nmea_parse(dataLine);
    // displayGPSData();

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
      //M5.Lcd.print("ALT:  ");
      //M5.Lcd.println(gps.altitude.feet());
    }
    if (gps.speed.isUpdated()) {
      shipDataModel.navigation.speed_over_ground_kn.kn = gps.speed.knots();
      shipDataModel.navigation.speed_over_ground_kn.age = millis();
    }
    if (gps.course.isUpdated()) {
      shipDataModel.navigation.course_over_ground_true.deg = gps.course.deg();
      shipDataModel.navigation.course_over_ground_true.age = millis();
    }
    if (gps.time.isUpdated()) {
      //shipDataModel.environment.time_gps.t = gps_time_to_tm(gps.date, gps.time);
      shipDataModel.environment.time_gps.age = millis();
    }
    if (headingTrue.isUpdated() && headingTrue.isValid()) {
      shipDataModel.navigation.heading_true.deg = parse_float(headingTrue.value());
      shipDataModel.navigation.heading_true.age = millis();
    }
    if (headingMag.isUpdated()) {
      shipDataModel.navigation.heading_mag.deg = parse_float(headingMag.value());
      shipDataModel.navigation.heading_mag.age = millis();
    }
    if (windAngle.isUpdated() && windReference.isUpdated()) {
      if (strcmp("R", windReference.value()) == 0) {
        shipDataModel.environment.wind.apparent_wind_angle.deg = parse_float(windAngle.value());
        shipDataModel.environment.wind.apparent_wind_angle.age = millis();
      }
      if (strcmp("T", windReference.value()) == 0) {
        shipDataModel.environment.wind.true_wind_angle.deg = parse_float(windAngle.value());
        shipDataModel.environment.wind.true_wind_angle.age = millis();
      }
    }
    if (windAngleI.isUpdated() && windReferenceI.isUpdated()) {
      if (strcmp("R", windReferenceI.value()) == 0) {
        shipDataModel.environment.wind.apparent_wind_angle.deg = parse_float(windAngleI.value());
        shipDataModel.environment.wind.apparent_wind_angle.age = millis();
      }
      if (strcmp("T", windReferenceI.value()) == 0) {
        shipDataModel.environment.wind.true_wind_angle.deg = parse_float(windAngleI.value());
        shipDataModel.environment.wind.true_wind_angle.age = millis();
      }
    }
    return found;
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

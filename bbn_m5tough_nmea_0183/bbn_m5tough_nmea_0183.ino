#include <M5Tough.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#undef min(a, b)
#include <ReactESP.h>

#include "TinyGPSPlus.h"

Preferences preferences;
String wifi_ssid;      // Store the name of the wireless network.
String wifi_password;  // Store the password of the wireless network.

WiFiClient nmeaClient;

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

using namespace reactesp;
ReactESP app;

void setup() {
  M5.begin();
  M5.Lcd.setTextWrap(true, true);
  M5.Lcd.setTextColor(YELLOW);

  Serial.begin(115200);
  while (!Serial) continue;
  delay(500);

  preferences.begin("wifi-config");
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");

  int attemptsCount = 30;
  int status = WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(wifi_ssid);
    status = WiFi.status();
    // wait .5 seconds for connection:
    delay(500);
    M5.Lcd.print(".");
    attemptsCount--;
    if (attemptsCount == 0) return;
  }
  if (status == WL_CONNECTED) {
    M5.Lcd.println("");
    M5.Lcd.println("Connected to " + wifi_ssid);
  }

  const char* host = "192.168.1.34";  //"lysmarine";
  int port = 10110;                   // NMEA 0183

  // Connect to the SignalK TCP server
  if (nmeaClient.connect(host, port)) {
    M5.Lcd.print("Connected to ");
    M5.Lcd.println(host);

    nmea_subscribe(nmeaClient);

  } else {
    M5.Lcd.println("Connection failed.");
    return;
  }
}

void loop() {
  M5.update();
  app.tick();
}

int samples = 25;

void nmea_subscribe(WiFiClient& client) {
  delay(50);
  //String dataFeed = client.readStringUntil('\n');
  //M5.Lcd.println(dataFeed);
  //delay(50);

  app.onAvailable(client, [samples, &client]() {
    while (client.connected() && client.available()) {
      String dataLine = client.readStringUntil('\n');
      bool found = nmea_parse(dataLine);
      if (found) {
        displayGPSData();
        samples--;
        if (samples <= 0) {
          client.stop();
        }
      }
    }
    delay(1);
  });
}

bool nmea_parse(String& line) {
  //M5.Lcd.println(line);
  parse_sentence(line.c_str());
  return line.length() > 0;
}

void parse_sentence(const char* line) {
  for (; *line != '\0'; line++) {
    gps.encode(*line);
  }
}

float parse_float(const char* str) {
  return strtof(str, NULL);
}

void displayGPSData() {

  boolean locValid = gps.location.isValid() && gps.location.isUpdated();
  if (locValid) {
    M5.Lcd.print("POS: ");
    M5.Lcd.print(gps.location.lat());
    M5.Lcd.print(" ");
    M5.Lcd.println(gps.location.lng());
  }

  boolean altValid = gps.altitude.isValid() && gps.altitude.isUpdated();
  if (altValid) {
    M5.Lcd.print("ALT: ");
    M5.Lcd.println(gps.altitude.feet());
  }

  boolean speedValid = gps.speed.isValid() && gps.speed.isUpdated();
  if (speedValid) {
    M5.Lcd.print("SOG: ");
    M5.Lcd.println(gps.speed.knots());
  }

  boolean courseValid = gps.course.isValid() && gps.course.isUpdated();
  if (courseValid) {
    M5.Lcd.print("COG: ");
    M5.Lcd.println(gps.course.deg());
  }

  boolean timeValid = gps.time.isValid() && gps.time.isUpdated();
  if (timeValid) {
    M5.Lcd.print("TIME: ");
    M5.Lcd.print(gps.time.hour());
    M5.Lcd.print(":");
    M5.Lcd.print(gps.time.minute());
    M5.Lcd.print(":");
    M5.Lcd.println(gps.time.second());
  }

  boolean headingTrueValid = headingTrue.isValid() && headingTrue.isUpdated();
  if (headingTrueValid) {
    M5.Lcd.print("HDT: ");
    M5.Lcd.println(headingTrue.value());
  }

  boolean headingMagValid = headingMag.isValid() && headingMag.isUpdated();
  if (headingMagValid) {
    M5.Lcd.print("HDM: ");
    M5.Lcd.println(headingMag.value());
  }

  boolean customValValid = customData_Value.isValid() && customData_Value.isUpdated();
  if (customValValid && customData_Type.isValid()) {    
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

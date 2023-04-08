#ifndef NET_MDNS_H
#define NET_MDNS_H

//   from command line you can discover services:
//   avahi-browse -ar

#ifdef __cplusplus
extern "C" {
#endif

  void browseServiceMDS(const char* service, const char* proto) {
    M5.Lcd.printf("Scan _%s._%s.local. ... ", service, proto);
    int n = MDNS.queryService(service, proto);
    if (n == 0) {
      M5.Lcd.println("no found");
    } else {
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print(n);
      M5.Lcd.println(" found");
      M5.Lcd.setTextColor(YELLOW);
      for (int i = 0; i < n; ++i) {
        // Print details for each service found
        M5.Lcd.print(" ");
        M5.Lcd.print(i + 1);
        M5.Lcd.print(": ");
        M5.Lcd.print(MDNS.hostname(i));
        M5.Lcd.print(" (");
        M5.Lcd.print(MDNS.IP(i));
        M5.Lcd.print(":");
        M5.Lcd.print(MDNS.port(i));
        M5.Lcd.println(")");
      }
    }
    M5.Lcd.println();
  }

  void discoverBasics() {
    browseServiceMDS("http", "tcp");
    delay(100);
    browseServiceMDS("nmea-0183", "tcp");
    delay(100);
    browseServiceMDS("signalk-http", "tcp");
    delay(100);
    browseServiceMDS("signalk-tcp", "tcp");
    delay(100);
    browseServiceMDS("signalk-ws", "tcp");
    delay(100);
    browseServiceMDS("pypilot", "tcp");
    delay(100);
    browseServiceMDS("mopidy-http", "tcp");
    delay(100);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

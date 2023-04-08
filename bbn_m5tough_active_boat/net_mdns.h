#ifndef NET_MDNS_H
#define NET_MDNS_H

//   from command line you can discover services:
//   avahi-browse -ar

#ifdef __cplusplus
extern "C" {
#endif

#define SK_TCP_HOST_PREF "signalk_tcp.host"
#define SK_TCP_PORT_PREF "signalk_tcp.port"

#define PYP_TCP_HOST_PREF "pypilot_tcp.host"
#define PYP_TCP_PORT_PREF "pypilot_tcp.port"

#define NMEA0183_TCP_HOST_PREF "nmea0183_tcp.host"
#define NMEA0183_TCP_PORT_PREF "nmea0183_tcp.port"

  void discover_n_config() {
    preferences.begin("clients-config");

    String signalk_tcp_host = preferences.getString(SK_TCP_HOST_PREF);
    int signalk_tcp_port = preferences.getInt(SK_TCP_PORT_PREF);
    if (signalk_tcp_host.length() <= 0 || signalk_tcp_port <= 0) {
      int n = MDNS.queryService("signalk-tcp", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(SK_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putString(SK_TCP_PORT_PREF, String(MDNS.port(0)));
        }
      }
    }
  }

  void browseServiceMDNS(const char* service, const char* proto) {
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
    browseServiceMDNS("http", "tcp");
    delay(100);
    browseServiceMDNS("nmea-0183", "tcp");
    delay(100);
    browseServiceMDNS("signalk-http", "tcp");
    delay(100);
    browseServiceMDNS("signalk-tcp", "tcp");
    delay(100);
    browseServiceMDNS("signalk-ws", "tcp");
    delay(100);
    browseServiceMDNS("pypilot", "tcp");
    delay(100);
    browseServiceMDNS("mpd", "tcp");
    delay(100);
    browseServiceMDNS("mopidy-http", "tcp");
    delay(100);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

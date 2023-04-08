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

#define MPD_TCP_HOST_PREF "mpd_tcp.host"
#define MPD_TCP_PORT_PREF "mpd_tcp.port"

  void mdns_begin() {
    if (!MDNS.begin("ESP32_Browser")) {
      M5.Lcd.println("Error setting up MDNS responder!");
    }
  }

  void mdns_end() {
    MDNS.end();
  }

  void discover_n_config() {
    preferences.begin("clients-config");

    String signalk_tcp_host = preferences.getString(SK_TCP_HOST_PREF);
    int signalk_tcp_port = preferences.getInt(SK_TCP_PORT_PREF);
    if (signalk_tcp_host.length() <= 0 || signalk_tcp_host == "0.0.0.0" || signalk_tcp_port <= 0) {
      int n = MDNS.queryService("signalk-tcp", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(SK_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putInt(SK_TCP_PORT_PREF, MDNS.port(0));
          M5.Lcd.print(MDNS.IP(0).toString());
          M5.Lcd.print(":");
          M5.Lcd.printf("%d - signalk\n", MDNS.port(0));
        }
      }
    }

    String nmea0183_tcp_host = preferences.getString(NMEA0183_TCP_HOST_PREF);
    int nmea0183_tcp_port = preferences.getInt(NMEA0183_TCP_PORT_PREF);
    if (nmea0183_tcp_host.length() <= 0 || nmea0183_tcp_host == "0.0.0.0" || nmea0183_tcp_port <= 0) {
      int n = MDNS.queryService("nmea-0183", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(NMEA0183_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putInt(NMEA0183_TCP_PORT_PREF, MDNS.port(0));
          M5.Lcd.print(MDNS.IP(0).toString());
          M5.Lcd.print(":");
          M5.Lcd.printf("%d - nmea0183\n", MDNS.port(0));
        } else {
          const char* found10110 = NULL;
          for (int i = 0; i < n; ++i) {
            if (MDNS.port(i) == 10110) {
              found10110 = MDNS.IP(i).toString().c_str();
            }
            break;
          }
          if (found10110 != NULL) {
            preferences.putString(NMEA0183_TCP_HOST_PREF, found10110);
            preferences.putInt(NMEA0183_TCP_PORT_PREF, 10110);
            M5.Lcd.print(found10110);
            M5.Lcd.print(":");
            M5.Lcd.printf("%d - nmea0183\n", 10110);
          } else {
            preferences.putString(NMEA0183_TCP_HOST_PREF, MDNS.IP(0).toString());
            preferences.putInt(NMEA0183_TCP_PORT_PREF, MDNS.port(0));
            M5.Lcd.print(MDNS.IP(0).toString());
            M5.Lcd.print(":");
            M5.Lcd.printf("%d - nmea0183\n", MDNS.port(0));
          }
        }
      }
    }

    String pypilot_tcp_host = preferences.getString(PYP_TCP_HOST_PREF);
    int pypilot_tcp_port = preferences.getInt(PYP_TCP_PORT_PREF);
    if (pypilot_tcp_host.length() <= 0 || pypilot_tcp_host == "0.0.0.0" || pypilot_tcp_port <= 0) {
      int n = MDNS.queryService("pypilot", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(PYP_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putString(PYP_TCP_PORT_PREF, String(MDNS.port(0)));
          M5.Lcd.print(MDNS.IP(0).toString());
          M5.Lcd.print(":");
          M5.Lcd.printf("%d - pypilot\n", MDNS.port(0));
        }
      }
    }

    String mpd_tcp_host = preferences.getString(MPD_TCP_HOST_PREF);
    int mpd_tcp_port = preferences.getInt(MPD_TCP_PORT_PREF);
    if (mpd_tcp_host.length() <= 0 || mpd_tcp_host == "0.0.0.0" || mpd_tcp_port <= 0) {
      int n = MDNS.queryService("mpd", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(MPD_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putString(MPD_TCP_PORT_PREF, String(MDNS.port(0)));
          M5.Lcd.print(MDNS.IP(0).toString());
          M5.Lcd.print(":");
          M5.Lcd.printf("%d - mpd\n", MDNS.port(0));
        }
      }
    }

    preferences.end();
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

#ifndef NET_MDNS_H
#define NET_MDNS_H

//   from command line you can discover services:
//   avahi-browse -ar

#ifdef __cplusplus
extern "C" {
#endif

  static const char* PROGMEM SK_TCP_HOST_PREF = "signalk_host";
  static const char* PROGMEM SK_TCP_PORT_PREF = "signalk_port";

  static const char* PROGMEM SK_HTTP_HOST_PREF = "sk_http_host";
  static const char* PROGMEM SK_HTTP_PORT_PREF = "sk_http_port";

  static const char* PROGMEM PYP_TCP_HOST_PREF = "pypilot_host";
  static const char* PROGMEM PYP_TCP_PORT_PREF = "pypilot_port";

  static const char* PROGMEM NMEA0183_TCP_HOST_PREF = "n0183_host";
  static const char* PROGMEM NMEA0183_TCP_PORT_PREF = "n0183_port";

  static const char* PROGMEM MPD_TCP_HOST_PREF = "mpd_host";
  static const char* PROGMEM MPD_TCP_PORT_PREF = "mpd_port";

  static const char* PROGMEM VENUS_MQTT_HOST_PREF = "ve_mqtt_host";
  static const char* PROGMEM VENUS_MQTT_PORT_PREF = "ve_mqtt_port";

  static bool mdns_up = false;

  bool mdns_begin() {
    if (!MDNS.begin("ESP32_Browser")) {
      return false;
    } else {
      mdns_up = true;
      return true;
    }
  }

  void mdns_end() {
    MDNS.end();
    mdns_up = false;
  }

  int mdns_query_svc(const char* service, const char* proto) {
    bool fail = false;
    if (!mdns_up) {
      fail = !mdns_begin();
    }
    if (!fail) {
      delay(50);
      return MDNS.queryService(service, proto);
    } else {
      return 0;
    }
  }

  bool discover_n_config() {
    bool saved = false;

    String signalk_tcp_host = preferences.getString(SK_TCP_HOST_PREF);
    int signalk_tcp_port = preferences.getInt(SK_TCP_PORT_PREF);
    if (signalk_tcp_host.length() <= 0 || signalk_tcp_host == "0.0.0.0" || signalk_tcp_port <= 0) {
      int n = mdns_query_svc("signalk-tcp", "tcp");
      if (n > 0) {
        preferences.putString(SK_TCP_HOST_PREF, MDNS.IP(0).toString());
        preferences.putInt(SK_TCP_PORT_PREF, MDNS.port(0));
        saved = true;
      }
    }

    String signalk_http_host = preferences.getString(SK_HTTP_HOST_PREF);
    int signalk_http_port = preferences.getInt(SK_HTTP_PORT_PREF);
    if (signalk_http_host.length() <= 0 || signalk_http_host == "0.0.0.0" || signalk_http_port <= 0) {
      int n = mdns_query_svc("signalk-http", "tcp");
      if (n > 0) {
        preferences.putString(SK_HTTP_HOST_PREF, MDNS.IP(0).toString());
        preferences.putInt(SK_HTTP_PORT_PREF, MDNS.port(0));
        saved = true;
      }
    }

    String nmea0183_tcp_host = preferences.getString(NMEA0183_TCP_HOST_PREF);
    int nmea0183_tcp_port = preferences.getInt(NMEA0183_TCP_PORT_PREF);
    if (nmea0183_tcp_host.length() <= 0 || nmea0183_tcp_host == "0.0.0.0" || nmea0183_tcp_port <= 0 || nmea0183_tcp_port != 10110) {
      int n = mdns_query_svc("nmea-0183", "tcp");
      if (n > 0) {
        if (n == 1) {
          preferences.putString(NMEA0183_TCP_HOST_PREF, MDNS.IP(0).toString());
          preferences.putInt(NMEA0183_TCP_PORT_PREF, MDNS.port(0));
          saved = true;
        } else {
          String found10110;
          for (int i = 0; i < n; ++i) {
            if (MDNS.port(i) == 10110) {
              found10110 = MDNS.IP(i).toString();
            }
            break;
          }
          if (found10110 != NULL && found10110.length() > 0) {
            preferences.putString(NMEA0183_TCP_HOST_PREF, found10110);
            preferences.putInt(NMEA0183_TCP_PORT_PREF, 10110);
            saved = true;
          } else {
            preferences.putString(NMEA0183_TCP_HOST_PREF, MDNS.IP(0).toString());
            preferences.putInt(NMEA0183_TCP_PORT_PREF, MDNS.port(0));
            saved = true;
          }
        }
      }
    }

    String pypilot_tcp_host = preferences.getString(PYP_TCP_HOST_PREF);
    int pypilot_tcp_port = preferences.getInt(PYP_TCP_PORT_PREF);
    if (pypilot_tcp_host.length() <= 0 || pypilot_tcp_host == "0.0.0.0" || pypilot_tcp_port <= 0) {
      int n = mdns_query_svc("pypilot", "tcp");
      if (n > 0) {
        preferences.putString(PYP_TCP_HOST_PREF, MDNS.IP(0).toString());
        preferences.putInt(PYP_TCP_PORT_PREF, MDNS.port(0));
        saved = true;
      }
    }

    String mpd_tcp_host = preferences.getString(MPD_TCP_HOST_PREF);
    int mpd_tcp_port = preferences.getInt(MPD_TCP_PORT_PREF);
    if (mpd_tcp_host.length() <= 0 || mpd_tcp_host == "0.0.0.0" || mpd_tcp_port <= 0) {
      int n = mdns_query_svc("mpd", "tcp");
      if (n > 0) {
        preferences.putString(MPD_TCP_HOST_PREF, MDNS.IP(0).toString());
        preferences.putInt(MPD_TCP_PORT_PREF, MDNS.port(0));
        saved = true;
      }
    }

    String venus_mqtt_host = preferences.getString(VENUS_MQTT_HOST_PREF);
    int venus_mqtt_port = preferences.getInt(VENUS_MQTT_PORT_PREF);
    if (venus_mqtt_host.length() <= 0 || venus_mqtt_host == "0.0.0.0" || venus_mqtt_port <= 0) {
      int n = mdns_query_svc("mqtt", "tcp");
      if (n > 0) {
        preferences.putString(VENUS_MQTT_HOST_PREF, MDNS.IP(0).toString());
        preferences.putInt(VENUS_MQTT_PORT_PREF, MDNS.port(0));
        saved = true;
      }
    }
    if (mdns_up) {
      mdns_end();
    }
    return saved;
  }

  void erase_mdns_lookups() {
    preferences.remove(VENUS_MQTT_HOST_PREF);
    preferences.remove(NMEA0183_TCP_HOST_PREF);
    preferences.remove(PYP_TCP_HOST_PREF);
    preferences.remove(SK_TCP_HOST_PREF);
    preferences.remove(SK_HTTP_HOST_PREF);
    preferences.remove(MPD_TCP_HOST_PREF);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef NET_PYPILOT_H
#define NET_PYPILOT_H

#ifdef __cplusplus
extern "C" {
#endif

  /*
   
  TCP client on port 23322:

  Just one line commands like this:

  ap.heading_command=220
  ap.enabled=true
  ap.enabled=false
  watch={"ap.heading":0.5}
  watch={"ap.mode":true}

  */

  static const char* PROGMEM AP_MODE_COMPASS = "MODE (Compass)";
  static const char* PROGMEM AP_MODE_GPS = "MODE (GPS)";
  static const char* PROGMEM AP_MODE_WIND = "MODE (Wind)";
  static const char* PROGMEM AP_MODE_WIND_TRUE = "MODE (True Wind)";

  void pypilot_greet(WiFiClient& client) {
    client.println(F("watch={\"ap.heading\":0.5}"));
    client.println(F("watch={\"ap.heading_command\":true}"));
    client.println(F("watch={\"ap.enabled\":true}"));
    client.println(F("watch={\"ap.mode\":true}"));
    client.flush();
  }

  void pypilot_send_engage(WiFiClient& client) {
    if (client.connected()) {
      client.println(F("ap.enabled=true"));
      client.flush();
    }
  }

  void pypilot_send_disengage(WiFiClient& client) {
    if (client.connected()) {
      client.println(F("ap.enabled=false"));
      client.flush();
    }
  }

  void pypilot_send_command(WiFiClient& client, float heading) {
    if (client.connected()) {
      client.print(F("ap.heading_command="));
      client.println(String(heading, 1));
      client.flush();
    }
  }

  void pypilot_send_mode(WiFiClient& client, const char* mode) {
    if (client.connected()) {
      if (strcmp(AP_MODE_GPS, mode) == 0) {
        client.println(F("ap.mode=\"gps\""));
      } else if (strcmp(AP_MODE_WIND, mode) == 0) {
        client.println(F("ap.mode=\"wind\""));
      } else if (strcmp(AP_MODE_WIND_TRUE, mode) == 0) {
        client.println(F("ap.mode=\"true wind\""));
      } else {
        client.println(F("ap.mode=\"compass\""));
      }
      client.flush();
    }
  }

  void setup_pypilot_reconnect(WiFiClient& client, const char* host, int port) {
    app.onRepeat(5000, [&client, host, port]() {
      if (!client.connected()) {
        setKeepAlive(client);
        if (client.connect(host, port, 3000)) {
          pypilot_greet(client);
        }
      }
    });
  }

  void pypilot_subscribe(WiFiClient& client) {

    pypilot_greet(client);

    app.onAvailable(client, [&client]() {
      while (client.available() > 8 /* Very important for performance and responsiveness */ && client.connected()) {
        bool found = pypilot_parse(client);
        if (found) {
          break; /* Very important for performance and responsiveness */
        }
      }
    });
  }

  void pypilot_begin(WiFiClient& pypClient, const char* pyp_host, int pyp_port) {
    setKeepAlive(pypClient);
    setup_pypilot_reconnect(pypClient, pyp_host, pyp_port);
    if (pypClient.connect(pyp_host, pyp_port)) {
      pypilot_subscribe(pypClient);
    }
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

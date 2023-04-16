#ifndef NET_PYPILOT_H
#define NET_PYPILOT_H

#ifdef __cplusplus
extern "C" {
#endif

  /*
   
  Uses Socket.IO over websockets
  
  #include <Arduino.h>
  #include <M5Tough.h>
  #include <WiFi.h>
  #include <WiFiClientSecure.h>
  #include <ArduinoJson.h>
  #include <WebSocketsClient.h>
  #include <SocketIOclient.h>

  ws://lysmarine:8080/socket.io/?EIO=4&transport=websocket&sid=Umq_p3uti3tZNSv2AAAG

  Requests:
  
  2probe
  5
  42["pypilot","watch={\"ap.heading\":0.5}"]
  42["pypilot","watch={\"ap.heading_command\":0.5}"]
  42["pypilot","watch={\"ap.mode\":true}"]
  42["pypilot","watch={\"ap.enabled\":true}"]
  42["ping"]
  42["pypilot","ap.heading_command=198.337"]
  42["pypilot","ap.mode=\"gps\""]
  42["pypilot","ap.mode=\"true\""]
  42["pypilot","ap.mode=\"true wind\""]
  42["pypilot","ap.mode=\"wind\""]
  42["pypilot","{\"ap.mode\":\"compass\",\"ap.heading_command\":164.3069}"]

  Responses:

  3probe
  42["pypilot","{\"ap.heading\":164.479}"]
  42["pong"]
  42["pypilot","{\"ap.heading_command\":198.337,\"ap.heading\":164.615}"]
  42["pypilot","{\"ap.mode\":\"compass\"}"]

  TCP client on port 23322:

  Just one line commands like this:

  ap.heading_command=220
  ap.enabled=true
  ap.enabled=false
  watch={"ap.heading":0.5}
  watch={"ap.mode":true}

  */

#define AP_MODE_COMPASS "MODE (Compass)"
#define AP_MODE_GPS "MODE (GPS)"
#define AP_MODE_WIND "MODE (Wind)"
#define AP_MODE_WIND_TRUE "MODE (True Wind)"

  void pypilot_greet(WiFiClient& client) {
    const char* data1 = "watch={\"ap.heading\":0.5}";
    client.println(data1);
    const char* data2 = "watch={\"ap.heading_command\":true}";
    client.println(data2);
    const char* data3 = "watch={\"ap.enabled\":true}";
    client.println(data3);
    const char* data4 = "watch={\"ap.mode\":true}";
    client.println(data4);
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
      client.println(String("ap.heading_command=") + String(heading, 1));
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

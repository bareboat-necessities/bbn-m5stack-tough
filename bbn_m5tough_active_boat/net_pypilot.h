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


  */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

#ifndef NET_PYPILOT_H
#define NET_PYPILOT_H

#ifdef __cplusplus
extern "C" {
#endif

  /*

  ws://lysmarine:8080/socket.io/?EIO=4&transport=websocket&sid=Umq_p3uti3tZNSv2AAAG

  Requests:

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

  42["pypilot","{\"ap.heading\":164.479}"]
  42["pong"]
  42["pypilot","{\"ap.heading_command\":198.337,\"ap.heading\":164.615}"]
  42["pypilot","{\"ap.mode\":\"compass\"}"]
  

  */



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

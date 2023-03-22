/**
 * @brief Example and test program for using SensESP.
 */

/*
  Required libraries

   mairas/ReactESP@^2.0.0
   me-no-dev/ESPAsyncTCP by dvarrel
   me-no-dev/ESP Async Webserver by dvarrel https://github.com/me-no-dev/ESPAsyncWebserver  https://github.com/me-no-dev/ESPAsyncWebServer/archive/refs/heads/master.zip
   alanswx/ESPAsyncWifiManager   https://github.com/alanswx/ESPAsyncWifiManager 
   bblanchon/ArduinoJson
   links2004/WebSockets https://github.com/Links2004/arduinoWebSockets
   pfeerick/elapsedMillis
   https://github.com/JoaoLopesF/RemoteDebug.git

   add 
   
   #define A0 GPIO_NUM_34
   
   into ~/Arduino/libraries/SensESP/src/sensesp/sensors/analog_input.h
*/
#include <M5Tough.h>
#undef min(a, b)

#include <sensesp.h>
#include <sensesp_app.h>
#include <sensesp/net/discovery.h>
#include <sensesp/net/http_server.h>
#include <sensesp/net/networking.h>
#include <sensesp/net/ws_client.h>
#include <sensesp/signalk/signalk_delta_queue.h>
#include <sensesp/signalk/signalk_output.h>
#include <sensesp/signalk/signalk_value_listener.h>
#include <sensesp/system/lambda_consumer.h>
#include <sensesp_minimal_app_builder.h>

using namespace sensesp;

reactesp::ReactESP app;

// The setup function performs one-time application initialization.
void setup() {
   M5.begin();
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SensESPMinimalAppBuilder builder;
  SensESPMinimalApp *sensesp_app = builder.set_hostname("sensesp")->get_app();

  auto *networking = new Networking(
    "/system/net", "", "", SensESPBaseApp::get_hostname(), "thisisfine");
  auto *http_server = new HTTPServer();

  // create the SK delta object
  auto sk_delta_queue_ = new SKDeltaQueue();

  // create the websocket client
  auto ws_client_ = new WSClient("/system/sk", sk_delta_queue_, "", 0);

  ws_client_->connect_to(new LambdaConsumer<WSConnectionState>(
    [](WSConnectionState input) {
      debugD("WSConnectionState: %d", input);
    }));

  // create the MDNS discovery object
  auto mdns_discovery_ = new MDNSDiscovery();

  auto *system_status_controller = new SystemStatusController();
  
  // create a new SKListener for navigation.headingMagnetic
  auto hdg = new SKValueListener<float>("navigation.headingMagnetic");
  hdg->connect_to(new LambdaConsumer<float>([](float input) {
    debugD("Heading: %f", input);
  }));

  // print out free heap
  app.onRepeat(2000, []() {
    debugD("Free heap: %d", ESP.getFreeHeap());
  });

  // Start the SensESP application running
  sensesp_app->start();
}

// The loop function is called in an endless loop during program execution.
// It simply calls `app.tick()` which will then execute all reactions as needed.
void loop() {
  M5.update();
  app.tick();
}

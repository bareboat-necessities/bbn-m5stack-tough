# bbn-m5stack-tough

M5Stack Tough Arduino sketches

With LVGL UI

Mostly oriented for boating projects

In Arduino IDE you need to install few libraries:

- M5StackTough (download zip source from github and import library as zip file) https://github.com/m5stack/M5Tough
- LVGL (install 8.x version via Arduino IDE libraries manager, copy ~/Arduino/libraries/lvgl/lv_conf_template.h into ~/Arduino/libraries/lv_conf.h, replace ````#if 0```` with ````#if 1```` in lv_conf.h, enable font Montserrat 20 in it ````#define LV_FONT_MONTSERRAT_20 1````) https://github.com/lvgl/lvgl
- ReactESP (install 2.x version via Arduino IDE libraries manager) https://github.com/mairas/ReactESP
- ArduinoJson (install 6.x version via Arduino IDE libraries manager) https://arduinojson.org/


Related projects:

https://github.com/m5stack/M5Tough

https://github.com/JohnySeven/SensESPWatch

https://github.com/JohnySeven/TWatchSK

https://github.com/SignalK/SensESP

https://github.com/norbert-walter/esp32-nmea2000

https://github.com/hatlabs/SH-wg-firmware

https://github.com/hatlabs/SH-ESP32-nmea2000-gateway

https://github.com/andyrbarrow/SignalKM5StackInstrument

https://github.com/bareboat-necessities/my-bareboat/tree/master/m5-boat-display

https://github.com/andyrbarrow/MQTTNMEADisplay

https://github.com/bareboat-necessities/rust-modules/tree/main/lightning-detect

https://github.com/idreamsi/LoRa-GPS-Tracker

https://bareboat-necessities.github.io/my-bareboat/bareboat-math.html

https://github.com/BjarneBitscrambler/SignalK-Orientation

https://github.com/SensESP/ve.direct_mppt

https://github.com/SensESP/VEDirect

https://github.com/joelkoz/CommonSens/tree/main/signalk-smart-switch

https://github.com/mhaberler/SensESP-M5Stack




Possible future ehnancements:

- PyPilot Remote Control Screen
- Lightning Detector with Screen
- IMU (compass and attitude) to NMEA (2000 or 0183)
- Engine RPM, oil temp, voltage, etc Display
- Batteries Status, etc Display
- Wind Display
- Wind Rose from climatology data for current location and time Display
- Depth Display
- Barometer with History
- LoRa Tracker receiver and display
- SignalK Alarms Display
- Weather Display, Air/Water temp, pressure, humidity, air quality
- Tides and Currents Display
- AIS Display
- Sailboat Polar Diagram Display
- Victron Data Display
- Rudder Position, Rate of Turn Display
- Anchor Alarm Set up and Display
- Heel (Clinometer) and Pitch
- GPS Location, speed, heading (true/mag), VMG
- Nav Lights and Digital Switching Control
- Tanks levels display
- Windlass remote control, chain counter display
- Emergency steering Control by Autopilot
- Bow Truster Remote Control
- MOB Button
- Compass Display
- Clock Display
- Music Player Controls (Spotify/Mopidy, etc)
- Bilge Monitoring Screen
- Wave Height, G-Forces Display
- Local Sunset/Sunrise
- Fog Alarm, Fog Signals (reminders)
- AC/Fridge/Heater/Watermaker/Hot Water Tank monitoring
- Fridge/AC thermostat control
- Retrievable Keel Status
- Alarm Clock / Timer / Stopwatch
- Solar Panel/Generator (Wind/Prop/Diesel) Power Monitoring
- Cabin, Deck, Search, Underwater lights controls,Fans, Blower controls
- Shrouds Load Forces Display from sensors
- Hatches Status Display

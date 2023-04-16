# M5Stack Tough (ESP-32) Arduino sketches

M5Stack Tough (ESP-32) Arduino sketches

With LVGL UI

Mostly oriented for boating projects

## Register M5Stack Board in Arduino IDE

1. Add this URL
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
as additional in Arduino IDE board manager settings

2. Install esp32 (by Espressif) board via Board Manager

3. Pick 'M5Stack-Core2' as your board in IDE

4. Set port (Something line /dev/ttyACM1 or so, you will find out by ````ls -ltr /dev/tty*```` after connecting your M5Tough device to USB)


## Required Libraries

In Arduino IDE you need to install few libraries:

- M5Tough (download zip source from github https://github.com/m5stack/M5Tough/archive/refs/heads/master.zip and import library as zip file into Arduino IDE) https://github.com/m5stack/M5Tough
- LVGL (install 8.x version via Arduino IDE libraries manager, copy ~/Arduino/libraries/lvgl/lv_conf_template.h into ~/Arduino/libraries/lv_conf.h, replace ````#if 0```` with ````#if 1```` in lv_conf.h, enable font Montserrat 20 in it ````#define LV_FONT_MONTSERRAT_20 1````,
enable Enable LV_COLOR_SCREEN_TRANSP in lv_conf.h for rotations to work: ````#define LV_COLOR_SCREEN_TRANSP 1````) https://github.com/lvgl/lvgl
- ReactESP (install 2.x version via Arduino IDE libraries manager) https://github.com/mairas/ReactESP
- ArduinoJson (install 6.x version via Arduino IDE libraries manager) https://arduinojson.org/


## Related projects:

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

https://github.com/arduino-libraries

https://github.com/mhaberler/SensESP-M5Stack

https://github.com/wellenvogel/esp32-nmea2000

https://github.com/AK-Homberger/NMEA2000WifiGateway-with-ESP32

https://github.com/AK-Homberger/NMEA2000-SignalK-Gateway

https://github.com/AK-Homberger/NMEA2000-AIS-Gateway

https://github.com/AK-Homberger/ESP32-Evo-Remote-Pilot-NMEA2000

https://github.com/AK-Homberger/NMEA2000-TempPressure

https://github.com/wholybee/esp32-smartBMSdisplay

https://github.com/ChrisLizon/VictronSOCLCD

https://github.com/RalfJL/VE.Direct2MQTT

https://github.com/humppafreak/SensESP-PeetBrosWind

https://github.com/sharandac/My-TTGO-Watch


##  Possible future ehnancements:

- Autopilot / PyPilot Remote Control Screen :heavy_check_mark:
- Lightning Detector with Screen (AS3935 sensor or better)
- IMU (compass and attitude) to NMEA (2000 or 0183) :heavy_check_mark:
- Engine RPM, oil temp, voltage, fuel burn per nautical mile, etc Display
- Batteries Status, etc Display
- Wind Display (True/Apparent) :heavy_check_mark:
- Drift and set, speed through water (knot meter)
- Wind Rose from climatology data for current location and time Display
- Depth Display :heavy_check_mark:
- Barometer (+Temp Humidity) with History
- Dangerous gases alarms
- LoRa Tracker receiver and display
- SignalK Alarms Display
- Weather Display, Air/Water temp, pressure, humidity, air quality
- Tides and Currents Display
- AIS Display
- Sailboat Polar Diagram Display
- Victron Data Display
- Rudder Position, Rate of Turn Display :heavy_check_mark:
- Anchor Alarm Set up and Display
- Heel (Clinometer) and Pitch :heavy_check_mark:
- GPS Location, speed, heading (true/mag), VMG
- Nav Lights and Digital Switching Control (ex: NMEA 2000 PGN 127501 "Binary Status Report" and PGN 127502 "Binary Switch Control")
- Tanks levels display
- Windlass remote control, chain counter display
- Emergency steering Control by Autopilot
- Bow Truster Remote Control
- MOB Button
- Compass Display (True/Magnetic), Magnetic Variation for the location :heavy_check_mark:
- Clock Display :heavy_check_mark:
- Music Player Controls (Spotify/Mopidy, etc)
- Bilge Monitoring Screen
- Wave Height, G-Forces Display based on: https://bareboat-necessities.github.io/my-bareboat/bareboat-math.html
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
- Dinghy location (via LoRa and putting data into AIS stream)
- Ship bell sound
- Fish detected alarm (Do not know for sure how to implement that winner feature. It's possible though)
- Smart Discovery of available services using mDNS and other ways
- Weather forecast using location adjusted Zambretti algorithm, based on pressure and wind history, date time, and location.
- Calculate mileage as integral of avg SOG
- Predict open sea state based on tables from https://en.wikipedia.org/wiki/Wind_wave#Formation
- Calculate dew point, air dencity. Detect fog conditions from pressure/temp/humidity
- Calculate times of Local Sunset/Sunrise
- Long wave time signals decoding DCF77, HBG, ALS162, WWVB
- BMS displays ex: https://github.com/wholybee/esp32-smartBMSdisplay
- TimeZone from location 
- Power winches controls
- Power furler controls 
- UI to control fog horn signal, and horn in general


##  Interfaces:

- SignalK (partially :heavy_check_mark:)
- NMEA 0183 (partially :heavy_check_mark:)
- NMEA 2000
- PyPilot (:heavy_check_mark:)
- MPD
- NTP (:heavy_check_mark:)
- MQTT (for Victron and others)
- mDNS (partially :heavy_check_mark:)


# M5Stack Tough (ESP-32) Sailing Instruments Display and Autopilot Control

M5Stack Tough (ESP-32) Sailing Instruments Display and Autopilot Control Arduino sketches

With LVGL UI

Mostly oriented for boating projects (can be interesting to RV and off-grid living users,
or Arduino and esp32 or other microcontroller enthusiasts and hobbyists)

## Set up on a boat

* Turn on your boat instruments
* Boot your boat computer with SignalK. Make sure it is accessible on your local boat Wi-Fi.
* Optionally: Turn on your Victron Cerbo GX or pi running Victron Venus OS if you have any of those. Make sure it is accessible on your local boat Wi-Fi.
* Plugin m5stack tough into USB power and see it's booting into Wi-Fi settings screen.
* Pick your local boat Wi-Fi in m5stack menu and enter password to connect your m5stack to your boat Wi-Fi (not to your marina's Wi-Fi). Use 2.4GHz WiFi bands, do not use 5GHz (it's not designed for IoT) Do not use wifi hotspot on pi - it somehow doesn't work with esp32
* Reboot m5stack
* Make sure that the devices m5stack is connecting to are set up with STATIC IP addresses
* Optionally: For Vitron Cerbo you might need to edit ip address and port of your Victron MQTT server in m5stack connection settings. Make sure you set up MQTT broker on Cerbo side (passwordless and with no encryption)



## Screenshots

<p>
<img src="screenshots/wind.png" width="330">
<img src="screenshots/victron.png" width="330">
</p>
<p>
<img src="screenshots/gps.png" width="330">
<img src="screenshots/engine.png" width="330">
</p>
<p>
<img src="screenshots/rudder.png" width="330">
<img src="screenshots/sun.png" width="330">
</p>
<p>
<img src="screenshots/tanks.png" width="330">
<img src="screenshots/autopilot.png" width="330">
</p>
<p>
<img src="screenshots/clock.png" width="330">
<img src="screenshots/compass.png" width="330">
</p>
<p>
<img src="screenshots/bbn_logo.png" width="330">
<img src="screenshots/depth.png" width="330">
</p>
<p>
<img src="screenshots/heel.png" width="330">
<img src="screenshots/network.png" width="330">
</p>
<p>
<img src="screenshots/reboot.png" width="330">
<img src="screenshots/speed.png" width="330">
</p>
<p>
<img src="screenshots/trip_leg.png" width="330">
<img src="screenshots/vessel.png" width="330">
</p>
<p>
<img src="screenshots/wifi.png" width="330">
<img src="screenshots/wifi_passw.png" width="330">
</p>
<p>
<img src="screenshots/ip_edit.png" width="330">
<img src="screenshots/autopilot_mode.png" width="330">
</p>

https://github.com/bareboat-necessities/bbn-m5stack-tough/blob/main/screenshots/VIEW.md

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
enable Enable LV_COLOR_SCREEN_TRANSP in lv_conf.h for rotations to work: ````#define LV_COLOR_SCREEN_TRANSP 1````) https://github.com/lvgl/lvgl Also use ````#define LV_TICK_CUSTOM 1```` in lv_conf.h
- ReactESP (install 2.x version via Arduino IDE libraries manager) https://github.com/mairas/ReactESP
- ArduinoJson (install 6.x version via Arduino IDE libraries manager) https://arduinojson.org/

##  Implemented Interfaces:

- SignalK (partially :heavy_check_mark:)
- NMEA 0183 (partially :heavy_check_mark:)
- NMEA 2000
- PyPilot (:heavy_check_mark:)
- MPD
- NTP (:heavy_check_mark:)
- MQTT (for Victron and others) (:heavy_check_mark:)
- mDNS (:heavy_check_mark:)

## Sleep

Deep sleep with waking up by touch screen. Backlight is off during sleep

##  Implemented Displays and Instruments:

- Autopilot / PyPilot Remote Control Screen :heavy_check_mark:
- IMU (compass and attitude) :heavy_check_mark:
- Engine RPM, alternator voltage, oil temp display :heavy_check_mark:
- Batteries Status, etc Display :heavy_check_mark:
- Wind Display (Ground/(True?)/Apparent) :heavy_check_mark:
- Drift and set, speed through water (knot meter) :heavy_check_mark:
- Depth Display :heavy_check_mark:
- Victron Data Display :heavy_check_mark:
- Rudder Position, Rate of Turn Display :heavy_check_mark:
- Heel (Clinometer) and Pitch :heavy_check_mark:
- GPS Location, speed, heading (true/mag) :heavy_check_mark:
- Compass Display (True/Magnetic), Magnetic Variation for the location :heavy_check_mark:
- Clock Display :heavy_check_mark:
- Trip data display, VMG  (:heavy_check_mark:)
- Local Sunset/Sunrise :heavy_check_mark:
- Tanks levels display :heavy_check_mark:
- Boat Design parameters :heavy_check_mark:
- Smart Discovery of available services using mDNS and other ways :heavy_check_mark:
- Brightness, screen orientation settings :heavy_check_mark:
- WiFi set up, password on screen keyboard :heavy_check_mark:
- Data providers IP addresses editors :heavy_check_mark:
- Connections statuses :heavy_check_mark:
- Reboot, reset, power off :heavy_check_mark:

##  Possible future ehnancements:

- Lightning Detector with Screen (AS3935 sensor or better)
- Engine fuel burn per nautical mile, etc display
- Wind Rose from climatology data for current location and time Display
- Barometer (+Temp Humidity) with History
- Dangerous gases alarms
- LoRa Tracker receiver and display
- SignalK Alarms Display
- Weather Display, Air/Water temp, pressure, humidity, air quality, illuminance, UV index, water salinity
- Tides and Currents Display
- AIS Display
- Sailboat Polar Diagram Display
- Anchor Alarm Set up and Display
- Nav Lights and Digital Switching Control (ex: NMEA 2000 PGN 127501 "Binary Status Report" and PGN 127502 "Binary Switch Control")
- Windlass remote control, chain counter display
- Emergency steering Control by Autopilot
- Bow Truster Remote Control
- MOB Button
- Music Player Controls (Spotify/Mopidy, etc)
- Bilge Monitoring Screen
- Wave Height, G-Forces Display based on: https://bareboat-necessities.github.io/my-bareboat/bareboat-math.html
- Fog Alarm, Fog Signals (reminders)
- AC/Fridge/Heater/Watermaker/Hot Water Tank monitoring
- Fridge/AC thermostat control
- Retrievable Keel Status
- Alarm Clock / Timer / Stopwatch
- Solar Panel/Generator (Wind/Prop/Diesel) Power Monitoring
- Cabin, Deck, Search, Underwater lights controls, Fans, Blower controls, Solenoid status/control
- Shrouds Load Forces Display from sensors
- Hatches Status Display
- Dinghy location (via LoRa and putting data into AIS stream)
- Ship bell sound
- Fish detected alarm (Do not know for sure how to implement that winner feature. It's possible though)
- Weather forecast using location adjusted Zambretti algorithm, based on pressure and wind history, date time, and location.
- Calculate mileage as integral of avg SOG
- Predict open sea state based on tables from https://en.wikipedia.org/wiki/Wind_wave#Formation
- Calculate dew point, air dencity. Detect fog conditions from pressure/temp/humidity
- Long wave time signals decoding DCF77, HBG, ALS162, WWVB
- BMS displays ex: https://github.com/wholybee/esp32-smartBMSdisplay
- TimeZone from location 
- Power winches controls
- Power furler controls 
- UI to control fog horn signal, and horn in general

## Using command line to upload firmware

````
python3 "~/.arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.py" \
 --chip esp32 --port "/dev/ttyACM0" --baud 921600  \
 --before default_reset --after hard_reset write_flash  \
 -z --flash_mode dio --flash_freq 80m --flash_size 16MB \
 0x1000 "bbn_m5tough_active_boat.ino.bootloader.bin" \
 0x8000 "bbn_m5tough_active_boat.ino.partitions.bin" \
 0xe000 "~/.arduino15/packages/esp32/hardware/esp32/2.0.9/tools/partitions/boot_app0.bin" \
 0x10000 "bbn_m5tough_active_boat.ino.bin" 
````


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

https://github.com/jukolein/W132

https://github.com/RalfJL/VE.Direct2MQTT

https://github.com/humppafreak/SensESP-PeetBrosWind

https://github.com/sharandac/My-TTGO-Watch

https://github.com/spascual90/Fenix

https://github.com/KimBP/AIS/blob/master/Examples/AISDecode/AISDecode.ino

https://github.com/skpang/Teensy40_800x400_LCD50_LVGL_NMEA2000_mfd

https://github.com/bareboat-necessities/bbn-nmea200-m5atom


TackTick Micronet to NMEA DIY
https://github.com/Rodemfr/MicronetToNMEA

Seatalk (arduino mega). esp32 doesn't support 9-bit UART required for Seatalk1
https://github.com/yOyOeK1/seatalk4

LoRa anchor alarm
https://github.com/factoidforrest/anchor-alarm-lora

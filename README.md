# M5Stack Tough (ESP-32) Sailing Instruments Display and Autopilot Control

M5Stack Tough (ESP-32) Sailing Instruments Display and Autopilot Control Arduino sketches

<p align="center">
<img src="./images/BBN_m5tough.jpg?raw=true" alt="BBN esp32 Marine Display" style="width: 50%; height: auto;" />
</p>

<p align="center">
<img src="./images/BBN_esp32_marine_display.jpg?raw=true" alt="BBN Display on m5tough with LVGL" style="width: 40%; height: auto;" />
</p>

With LVGL UI

Mostly oriented for boating projects (can be interesting to RV and off-grid living users,
or Arduino and esp32 or other microcontroller enthusiasts and hobbyists)

## Set up on a boat

* Turn on your boat instruments
* Boot your boat computer with SignalK. Make sure it is accessible on your local boat Wi-Fi.
* Optionally: Turn on your Victron Cerbo GX or pi running Victron Venus OS if you have any of those. Make sure it is accessible on your local boat Wi-Fi.
* Plugin m5stack tough into USB power and see it's booting into Wi-Fi settings screen.
* Pick your local boat Wi-Fi in m5stack menu and enter password to connect your m5stack to your boat Wi-Fi (not to your marina's Wi-Fi). Use 2.4GHz WiFi bands, do not use 5GHz (it's not designed for IoT)
* Reboot m5stack
* Make sure that the devices m5stack is connecting to are set up with STATIC IP addresses
* Optionally: For Vitron Cerbo you might need to edit ip address and port of your Victron MQTT server in m5stack connection settings. Make sure you set up MQTT broker on Cerbo side (passwordless and with no encryption)

## M5Tough mount kit

Panel Frame for M5Core (SKU: A125)

https://shop.m5stack.com/products/panel-frame-for-m5core


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

Use (tested) Arduino IDE 1.8.x

1. Add this URL
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
as additional in Arduino IDE board manager settings

2. Install esp32 (by Espressif) version 2.0.15 board via Board Manager

3. Pick 'M5Stack-Core2' as your board in IDE

4. Set port (Something like /dev/ttyACM1 or so, you will find out by ````ls -ltr /dev/tty*```` after connecting your M5Tough device to USB)


## Required Libraries

In Arduino IDE you need to install few libraries:

- M5Tough (download zip source from github https://github.com/m5stack/M5Tough/archive/refs/heads/master.zip and import library as zip file into Arduino IDE) https://github.com/m5stack/M5Tough
- LVGL https://github.com/lvgl/lvgl (install 8.x version via Arduino IDE libraries manager, copy ~/Arduino/libraries/lvgl/lv_conf_template.h into ~/Arduino/libraries/lv_conf.h, replace ````#if 0```` with ````#if 1```` in lv_conf.h, enable font Montserrat 20 in it ````#define LV_FONT_MONTSERRAT_20 1````,
enable Enable LV_COLOR_SCREEN_TRANSP in lv_conf.h for rotations to work: ````#define LV_COLOR_SCREEN_TRANSP 1````)  Also use ````#define LV_TICK_CUSTOM 1```` in lv_conf.h
- ReactESP (install 2.x version via Arduino IDE libraries manager) https://github.com/mairas/ReactESP
- ArduinoJson (install 6.x version via Arduino IDE libraries manager) https://arduinojson.org/
- MQTT (2.5.x via Arduino IDE libraries manager) https://github.com/256dpi/arduino-mqtt

You can look at .github/workflows/build.yaml for specific versions this project was tested with

## Checking out code

````
git clone https://github.com/bareboat-necessities/bbn-m5stack-tough/
````

## Compiling and uploading code to the device

- In Arduino open the bbn-m5stack-tough/bbn_m5tough_active_boat/bbn-m5stack-tough.ino file
- Click 'Upload' icon (Looks like an arrow pointing right)
- It will take a while to compile and upload the sketch. Compilation might give some warnings which can be ignored.
- ESP32 device will reboot and ask you to choose a Wi-Fi network to connect to. Choose the one which has access to your boat LAN.
  
##  Implemented Interfaces:

- SignalK (partially :heavy_check_mark: )
- NMEA 0183 (partially :heavy_check_mark: )
- NMEA 2000
- PyPilot ( :heavy_check_mark: )
- MPD
- NTP ( :heavy_check_mark: )
- MQTT (for Victron and others)  ( :heavy_check_mark: )
- mDNS ( :heavy_check_mark: )

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
- Trip data display, VMG  ( :heavy_check_mark: )
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

## Download binaries

Main branch:

https://github.com/bareboat-necessities/bbn-m5stack-tough/releases/tag/main

## Using command line to upload firmware

Unzip downloaded archives

Examples:

### Linux:

````

python3 "~/.arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.py" \
 --chip esp32 --port "/dev/ttyACM0" --baud 921600  \
 --before default_reset --after hard_reset write_flash -z \
 --flash_mode dio --flash_freq 80m --flash_size 16MB \
 0x1000 "bbn_m5tough_active_boat.ino.bootloader.bin" \
 0x8000 "bbn_m5tough_active_boat.ino.partitions.bin" \
 0xe000 "~/.arduino15/packages/esp32/hardware/esp32/2.0.15/tools/partitions/boot_app0.bin" \
 0x10000 "bbn_m5tough_active_boat.ino.bin"

````

### BBN OS Linux:

Find out tty device for your connected m5stack device. Use it in commands below. Make sure with lsof command that nothing else 
holds the device file open.

Easy:

````
wget https://github.com/bareboat-necessities/bbn-m5stack-tough/releases/download/main/bbn_m5tough_active_boat_bin-2024-12-03.zip

unzip bbn_m5tough_active_boat_bin-2024-12-03.zip

/srv/esphome/bin/esptool.py \
 --chip esp32 --port "/dev/ttyACM1" --baud 921600 \
 --before default_reset --after hard_reset write_flash \
 0x0 bbn_m5tough_active_boat_firmware.bin
````

Or:

````
wget https://raw.githubusercontent.com/bareboat-necessities/my-bareboat/refs/heads/master/m5stack-tools/bbn-flash-m5-disp.sh
chmod +x bbn-flash-m5-disp.sh 
./bbn-flash-m5-disp.sh -p /dev/ttyACM3
````

Or:

````
/srv/esphome/bin/esptool.py \
 --chip esp32 --port "/dev/ttyACM1" --baud 921600  \
 --before default_reset --after hard_reset write_flash -z \
 --flash_mode dio --flash_freq 80m --flash_size 16MB \
 0x1000 "bbn_m5tough_active_boat.ino.bootloader.bin" \
 0x8000 "bbn_m5tough_active_boat.ino.partitions.bin" \
 0xe000 "$HOME/.arduino15/packages/esp32/hardware/esp32/2.0.15/tools/partitions/boot_app0.bin" \
 0x10000 "bbn_m5tough_active_boat.ino.bin"
````


### Windows:

````

<PATH>/esp32/tools/esptool_py/4.5.1/esptool.exe --chip esp32 --port COM5 --baud 921600 
 --before default_reset --after hard_reset write_flash -z 
 --flash_mode dio --flash_freq 80m --flash_size 16MB 
 0x1000 bbn_m5tough_active_boat.ino.bootloader.bin 
 0x8000 bbn_m5tough_active_boat.ino.partitions.bin 
 0xe000 <PATH>/esp32/hardware/esp32/2.0.15/tools/partitions/boot_app0.bin 
 0x10000 bbn_m5tough_active_boat.ino.bin 

````

## Project Home

https://bareboat-necessities.github.io/

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

https://github.com/speters/ClipperDuet2N2k

TackTick Micronet to NMEA DIY
https://github.com/Rodemfr/MicronetToNMEA

Seatalk (arduino mega). esp32 doesn't support 9-bit UART required for Seatalk1
https://github.com/yOyOeK1/seatalk4

Script that reads 9-bits seatalk 1 messages from any GPIO port using bitbanging, and converts it into NMEA0183 messages
https://github.com/marcobergman/seatalk_convert

LoRa anchor alarm
https://github.com/factoidforrest/anchor-alarm-lora

SeaTalk Autopilot Control
https://github.com/nauti-control/STWirelessRemote

https://github.com/Glinnes/NMEAParser

https://github.com/norbert-walter/esp32-nmea2000-obp60

https://github.com/rob42/FreeboardMega/tree/signal_k_dev

https://github.com/pypilot/pypilot_mfd

https://github.com/htool/ESP32_Precision-9_compass_CMPS14

https://github.com/gerryvel/NMEA2000-AIS-Gateway

https://github.com/gerryvel/N2k-Distribution

https://github.com/gerryvel/Bootsdaten

https://github.com/gerryvel/NMEA2000_TPW

https://github.com/gerryvel/Motordaten

NKE Topline protocol
https://github.com/KristianJerpetjon/NkeToplineCpp

#include <M5Tough.h>
#include <ArduinoBLE.h>

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEBroadcast);

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
const uint8_t completeRawAdvertisingData[] = { 0x02, 0x01, 0x06, 0x09, 0xff, 0x01, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };

void setup() {
  M5.begin();
  Serial.begin(9600);
  while (!Serial) {}

  if (!BLE.begin()) {
    M5.Lcd.println("failed to initialize BLE!");
    while (1) {}
  }

  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Build advertising data packet
  BLEAdvertisingData advData;
  // If a packet has a raw data parameter, then all the other parameters of the packet will be ignored
  advData.setRawData(completeRawAdvertisingData, sizeof(completeRawAdvertisingData));
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  // Build scan response data packet
  BLEAdvertisingData scanData;
  scanData.setLocalName("m5tough");
  // Copy set parameters in the actual scan response packet
  BLE.setScanResponseData(scanData);

  BLE.advertise();

  M5.Lcd.println("advertising ...");
}

void loop() {
  M5.update();
  BLE.poll();
}

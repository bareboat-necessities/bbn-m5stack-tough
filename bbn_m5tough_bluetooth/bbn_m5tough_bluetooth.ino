#include <M5Tough.h>
#include <ArduinoBLE.h>

void setup() {
  M5.begin();
  Serial.begin(9600);
  while (!Serial) {}

  // begin initialization
  if (!BLE.begin()) {
    M5.Lcd.println("starting Bluetooth® Low Energy module failed!");
  }

  M5.Lcd.println("Bluetooth® Low Energy Central - Peripheral Explorer");

  // start scanning for peripherals
  BLE.scan();
}

void loop() {
  M5.update();

  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    M5.Lcd.print("Found ");
    M5.Lcd.print(peripheral.address());
    M5.Lcd.print(" '");
    M5.Lcd.print(peripheral.hasLocalName());
    M5.Lcd.print("' ");
    M5.Lcd.print(peripheral.advertisedServiceUuid());
    M5.Lcd.println();

    // see if peripheral is a LED
    if (peripheral.localName() == "LED") {
      // stop scanning
      BLE.stopScan();

      explorerPeripheral(peripheral);
    }
  }
}

void explorerPeripheral(BLEDevice peripheral) {
  // connect to the peripheral
  M5.Lcd.println("Connecting ...");

  if (peripheral.connect()) {
    M5.Lcd.println("Connected");
  } else {
    M5.Lcd.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  M5.Lcd.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    M5.Lcd.println("Attributes discovered");
  } else {
    M5.Lcd.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // read and print device name of peripheral
  M5.Lcd.println();
  M5.Lcd.print("Device name: ");
  M5.Lcd.println(peripheral.deviceName());
  M5.Lcd.print("Appearance: 0x");
  M5.Lcd.println(peripheral.appearance(), HEX);
  M5.Lcd.println();

  // loop the services of the peripheral and explore each
  for (int i = 0; i < peripheral.serviceCount(); i++) {
    BLEService service = peripheral.service(i);

    exploreService(service);
  }

  M5.Lcd.println();

  // we are done exploring, disconnect
  M5.Lcd.println("Disconnecting ...");
  peripheral.disconnect();
  M5.Lcd.println("Disconnected");
}

void exploreService(BLEService service) {
  // print the UUID of the service
  M5.Lcd.print("Service ");
  M5.Lcd.println(service.uuid());

  // loop the characteristics of the service and explore each
  for (int i = 0; i < service.characteristicCount(); i++) {
    BLECharacteristic characteristic = service.characteristic(i);

    exploreCharacteristic(characteristic);
  }
}

void exploreCharacteristic(BLECharacteristic characteristic) {
  // print the UUID and properties of the characteristic
  M5.Lcd.print("\tCharacteristic ");
  M5.Lcd.print(characteristic.uuid());
  M5.Lcd.print(", properties 0x");
  M5.Lcd.print(characteristic.properties(), HEX);

  // check if the characteristic is readable
  if (characteristic.canRead()) {
    // read the characteristic value
    characteristic.read();

    if (characteristic.valueLength() > 0) {
      // print out the value of the characteristic
      M5.Lcd.print(", value 0x");
      printData(characteristic.value(), characteristic.valueLength());
    }
  }
  M5.Lcd.println();

  // loop the descriptors of the characteristic and explore each
  for (int i = 0; i < characteristic.descriptorCount(); i++) {
    BLEDescriptor descriptor = characteristic.descriptor(i);

    exploreDescriptor(descriptor);
  }
}

void exploreDescriptor(BLEDescriptor descriptor) {
  // print the UUID of the descriptor
  M5.Lcd.print("\t\tDescriptor ");
  M5.Lcd.print(descriptor.uuid());

  // read the descriptor value
  descriptor.read();

  // print out the value of the descriptor
  M5.Lcd.print(", value 0x");
  printData(descriptor.value(), descriptor.valueLength());

  M5.Lcd.println();
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    if (b < 16) {
      M5.Lcd.print("0");
    }

    M5.Lcd.print(b, HEX);
  }
}

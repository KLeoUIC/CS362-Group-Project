#include <ArduinoBLE.h>

BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic sendCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20);
BLEStringCharacteristic receiveCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20);

void setup() {
  Serial.begin(115200);
  
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("ESP32_Server");
  
  BLE.setAdvertisedService(dataService);
  dataService.addCharacteristic(sendCharacteristic);
  dataService.addCharacteristic(receiveCharacteristic);
  BLE.addService(dataService);
  
  sendCharacteristic.setValue("");
  receiveCharacteristic.setValue("");

  BLE.advertise();
  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    
    while (central.connected()) {
      if (sendCharacteristic.written()) {
        String value = sendCharacteristic.value();
        Serial.print("Received: ");
        Serial.println(value);
        
        receiveCharacteristic.setValue("Response to: " + value);
      }
      
      delay(10);
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
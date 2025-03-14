#include <ArduinoBLE.h>

// UUIDs - must match peripheral
const char* serviceUUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* sendCharUUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* receiveCharUUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

String messageToSend = "Hello from client";
bool sentMessage = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Starting BLE Client...");
  
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  
  Serial.println("BLE Central initialized, scanning for peripherals...");
}

void loop() {
  BLE.scanForUuid(serviceUUID);
  
  BLEDevice peripheral = BLE.available();
  
  if (peripheral) {
    Serial.print("Found peripheral: ");
    Serial.println(peripheral.localName());
    
    BLE.stopScan();
    
    if (connectToPeripheral(peripheral)) {
      Serial.println("Disconnected from server");
    } else {
      Serial.println("Connection failed");
    }
    
    // Restart scanning
    Serial.println("Scanning for peripherals...");
    sentMessage = false;
  }
}

bool connectToPeripheral(BLEDevice peripheral) {
  Serial.println("Connecting...");
  
  if (!peripheral.connect()) {
    return false;
  }
  
  Serial.println("Connected");
  
  // Discover peripheral attributes
  if (!peripheral.discoverAttributes()) {
    Serial.println("Attribute discovery failed");
    peripheral.disconnect();
    return false;
  }
  
  BLEService service = peripheral.service(serviceUUID);
  if (!service) {
    Serial.println("Service not found");
    peripheral.disconnect();
    return false;
  }
  
  BLECharacteristic sendChar = service.characteristic(sendCharUUID);
  BLECharacteristic receiveChar = service.characteristic(receiveCharUUID);
  
  if (!sendChar || !receiveChar) {
    Serial.println("Characteristics not found");
    peripheral.disconnect();
    return false;
  }
  
  if (!receiveChar.subscribe()) {
    Serial.println("Subscription failed");
    peripheral.disconnect();
    return false;
  }
  
  Serial.println("Subscribed to notifications");
  
  while (peripheral.connected()) {
    if (!sentMessage) {
      sendChar.writeValue(messageToSend);
      Serial.println("Message sent: " + messageToSend);
      sentMessage = true;
    }
    
    if (receiveChar.valueUpdated()) {
      String value = receiveChar.readValue();
      Serial.println("Received: " + value);
    }
    
    delay(100);
  }
   
  return true;
}
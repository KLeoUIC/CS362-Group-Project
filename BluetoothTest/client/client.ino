#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVER_NAME "ESP32_1"

static BLEAddress* serverAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == SERVER_NAME) {
      Serial.print("Found server device: ");
      Serial.println(advertisedDevice.toString().c_str());
      serverAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      BLEDevice::getScan()->stop();
    }
  }
};

static void notifyCallback(BLERemoteCharacteristic* pCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  Serial.print("Received notification: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)pData[i]);
  }
  Serial.println();
}

bool connectToServer() {
  Serial.print("Connecting to ");
  Serial.println(serverAddress->toString().c_str());
  
  BLEClient* pClient = BLEDevice::createClient();
  
  if (!pClient->connect(*serverAddress)) {
    Serial.println("Connection failed");
    return false;
  }
  
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.println("Failed to find service UUID");
    return false;
  }
  
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("Failed to find characteristic UUID");
    return false;
  }
  
  if (pRemoteCharacteristic->canRead()) {
    String value = pRemoteCharacteristic->readValue().c_str();
    Serial.print("Initial value: ");
    Serial.println(value);
  }
  
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
    Serial.println("Registered for notifications");
  }
  
  connected = true;
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false); // Scan for 5 seconds
}

void loop() {
  // If the flag is true, connect to the server
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Failed to connect");
    }
    doConnect = false;
  }
  
  // If connected, poll for messages
  if (connected) {
    // Read the latest value
    if (pRemoteCharacteristic->canRead()) {
      String value = pRemoteCharacteristic->readValue().c_str();
      Serial.print("Current value: ");
      Serial.println(value);
    }
  } else {
    // Restart scanning if disconnected
    BLEDevice::getScan()->start(5);
  }
  
  delay(1000);
}
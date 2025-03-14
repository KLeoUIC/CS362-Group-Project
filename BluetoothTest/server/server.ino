#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String receivedMessage = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Server"); 
  Serial.println("The device started in server mode, make sure remote BT device is on!");
}

void loop() {
  SerialBT.println("Hello from ESP32 server");
  delay(1000);
  
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      receivedMessage += String(incomingChar);
    } else {
      Serial.println("Message from client: " + receivedMessage);
      receivedMessage = "";
    }
  }
  delay(20);
}
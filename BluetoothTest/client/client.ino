#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String receivedMessage = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Client"); 
  Serial.println("The device started in client mode, waiting for connections...");
}

void loop() {
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      receivedMessage += String(incomingChar);
    } else {
      Serial.println("Message from server: " + receivedMessage);
      receivedMessage = "";
      SerialBT.println("Hello from ESP32 clietn");
    }
  }
  delay(20);
}
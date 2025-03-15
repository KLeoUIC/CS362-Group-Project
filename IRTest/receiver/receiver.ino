#include <IRremote.h>

#define IR_RECEIVER_PIN 18

void setup() {
 Serial.begin(9600);
 IrReceiver.begin(IR_RECEIVER_PIN); 
 Serial.println("IR Receiver initialized");
}

void loop() {
 if (IrReceiver.decode()) {
   Serial.print("IR Code: 0x");
   Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
   IrReceiver.resume();
 }
}

#include <IRremote.h>

#define txPin 22
#define buttonPin 23
#define LED_BUILTIN 2

bool buttonState = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(buttonPin, INPUT); 
    IrSender.begin(txPin);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, FALLING);
}

void buttonPressed() {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > debounceDelay) {
        buttonState = true;
        lastDebounceTime = currentTime;
    }
}

void loop() {
    if (buttonState) {
        Serial.println("Button was pressed! Sending IR code...");
        digitalWrite(LED_BUILTIN, HIGH);
        IrSender.sendNEC(0x20DF10EF, 32);
        buttonState = false;
        delay(100);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
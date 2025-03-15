# CS362-Group-Project
## Project Overview
Building laser tag blasters and vests for the IOT project for CS362 at UIC.

## Hardware Setup
## MISSING BLUETOOTH INITIALIZATION INSTRUCTIONS (WILL DO LATER.)

### IR Transmitter
1. Connect IR VCC pin to 3.3V rail
2. Connect IR GND pin to ground rail
3. Connect IR DAT pin to GPIO pin 22
2. Connect button to pin 23 and GND + pull-up resistor

### IR Receiver
1. Connect VCC pin to 3.3V on ESP32
2. Connect GND pin to ground rail
3. Connect OUT pin to GPIO pin 18

## Running the Code
1. Run the transmitter code on one ESP32
2. Run the receiver code on another ESP32
3. Press the button on the transmitter ESP32 to send IR signals
4. The receiver's serial monitor should display the received IR code


## Note:
1. If you want to change any of the pins, make sure to update the pin definitions in the code accordingly.
2. The IR codes are hardcoded in the code. If you want to change them, make sure to update the codes in both the transmitter and receiver code.
3. Everything you need is **online**. Just search for the components and you will find the datasheets and tutorials on how to use them.

## Troubleshooting
- If transmitter esp32 BUILTIN LED doesn't light up, check wiring and LED_BUILTIN pin (default: 2)
- If receiver doesn't detect signals, ensure direct line of sight
- Maximum range is limited by 3.3V power supply (I have not figured that out yet. If you do please state it here.)

## Development Environment Setup
### Mac OS
[Getting Started With ESP32 on a Mac](https://www.instructables.com/Getting-Started-With-ESP32-on-a-Mac/)
### Windows
[How to Set Up ESP32-WROOM-32](https://samueladesola.medium.com/how-to-set-up-esp32-wroom-32-b2100060470c)
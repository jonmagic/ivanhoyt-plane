# Feather M4 CAN

This folder contains the code and instructions for programming the Arduino Feather M4 CAN board, which is used in the avionics system of my dad's plane. The code is designed to interface with the CAN bus and handle communication with other avionics components.

## Pre-requisites

1. **Arduino IDE**: Ensure you have the latest version of the [Arduino IDE](https://www.arduino.cc/) installed.
2. Install the [Mavlink package](https://github.com/okalachev/mavlink-arduino) from the Arduino IDE Libraries section.
3. Install the [Adafruit CAN](https://github.com/adafruit/Adafruit_CAN) from the Arduino IDE Libraries section. Reference [these instructions](https://learn.adafruit.com/adafruit-feather-m4-can-express/arduino-can-examples) if you want to dive deeper.

## How to Use

1. **Build the Code**: Use the Arduino IDE or PlatformIO to compile the `mav-to-can.cpp` file.
2. **Upload to Board**: Connect the Feather M4 CAN board to your computer and upload the compiled code.
    > Power device on, press button once, press a second time for 1 second and let go and the light will flash and turn green when in DFU mode and ready to upload. After uploading, the board should automatically reset and start running the code and the light will turn off.

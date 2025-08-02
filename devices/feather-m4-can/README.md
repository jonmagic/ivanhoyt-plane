# Feather M4 CAN

This folder contains the code and instructions for programming the Arduino Feather M4 CAN board, which is used in the avionics system of my dad's plane. The code is designed to interface with the CAN bus and handle communication with other avionics components.

## How to Use

1. **Build the Code**: Use the Arduino IDE or PlatformIO to compile the `mav-to-can.cpp` file.
2. **Upload to Board**: Connect the Feather M4 CAN board to your computer and upload the compiled code.
3. **Configuration**: Ensure that the board is configured correctly to communicate with the CAN bus. This may involve setting up specific parameters in the code or using external configuration files.
4. **Testing**: After uploading, test the board to ensure it is correctly sending and receiving messages on the CAN bus. You can use a CAN bus analyzer or similar tool to monitor the traffic.
5. **Integration**: Integrate the board with other avionics components as per the wiring diagrams and instructions provided in the `docs/` directory.

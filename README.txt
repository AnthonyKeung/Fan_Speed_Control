# Fan Speed Controller

This Fan Speed Controller project involves the use of an STM32F070 Nucleo Development Board to control the speed of a DC fan.  
The user can choose between 3 modes of operation and tune a parameter in each mode to achieve the desired behaviour, seeing the response on the LCD.  
A temperature sensor and tachometer are used to provide information to the system and the user, providing an easy-to-understand fan controller system.


## Table of contents
- System Setup
- Initial Check
- Mode Selection
- Mode Display
- Mode Details
    - Closed Loop
    - Open Loop
    - Slow Speed Control

## System Setup
The development board should be connected to 5V power using a micro-USB cable.
The red and black banana sockets must be connected to 12V and 0V respectively using a power supply.

## Initial Check
Before the system can be used, a hardware check is initiated for the Temperature Sensor.
If the Temperature Sensor is not connected correctly, a red LED will light up, and a display message saying "TempSensor Fail" will appear on the LCD.
This message will flash until the Temperature Sensor issue is fixed and the system will enter the first mode(Closed Loop).

## Mode Selection
By pressing the blue button on the development board, the user can cycle through the three available modes: Closed Loop, Open Loop, and Slow Speed Control.
Each press of the button switches to the next mode in sequence, looping back to the first mode when pressed in the final mode.

## Mode Display
The 2 LEDs on the green extension board will display the current mode once the temperature sensor check is completed.
The LED labelled "D2" on the green extension board will be on by itself in Closed Loop mode, the other LED labelled "D1" will be on by itself in Open Loop mode, and both will be on in Slow Speed Control mode.

## Mode Details

### Closed Loop
Here the Fan Speed is controlled through a PID controller. The user can set the target temperature(TT) by adjusting the rotary encoder.
Initially, the target temperature will be set to 24. "TT = 24" will be displayed on the LCD and any time the rotary encoder is adjusted the new TT will be displayed.

Once the TT is set, Current Temperature(CT) and Speed(S) will be displayed in this format "T = 24 S = 683".
The fan speed will adjust accordingly to reach the TT.

Note that the TT will be kept despite changing modes.

### Open Loop
The fan's speed is directly controlled by the user by turning the knob attached to the rotary encoder.

The LCD will display the measured speed of the fan in revolutions per minute (RPM) on the left and the duty cycle (DC) of the fan on the right. Example: "S: 1000 DC: 30% ".
By turning the knob, the user can change the duty cycle, which controls how much power is delivered to the fan. 
Turning the knob clockwise will increase the duty cycle by 1% up to a limit of 100%, whereas turning it anticlockwise will decrease the duty cycle by 1% down to a limit of 20%.
The fan will spin at speeds between approximately 700 RPM and 1950 RPM.

### Slow Speed Control
The fan's speed is directly controlled by the user, at speeds below the rated fan speed on the datasheet of the fan.

Operating the system in this mode is similar to the Open Loop mode, but the duty cycle ranges from 4.0% to 20% and will change by 0.1% when the knob is rotated.
The fan's speed will range from approximately 75 RPM at 4.0% DC to 700 RPM at 20.0% DC;
The fan should be placed horizontally to prevent it from stalling at lower speeds.


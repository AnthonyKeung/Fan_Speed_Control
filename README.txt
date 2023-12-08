# Fan Speed Controller

The Fan Speed Controller repository contains the code needed to read, displays and controls the speed of a fan.
There are three control modes : open loop, closed loop and a slow speed control.

## Table of contents
- Initial Check
- Mode Display
-Mode Details
    - Closed Loop
    - Open Loop
    - Slow speed

- Troubleshooting

# Initial Check
Before the system begins a hardware check is iinitiated for the Temperature Sensor. If the Temperature Sensor is not connected a red LED and
display message saying "TempSensor Fail" on the LCD. This the message will flash until the Temperature Sensor is fixed and the system will enter the 
first mode.

# Mode Display 
2 LED's will display the mode in a binary representation. 1 is Closed, 2 is Open and 3 is Slow Control. 

##Usage

# Closed Loop
Here the Fan Speed is controlled through a PID controller. The user can set the target temperature by adjusting the
rotary encoder. Initially, the target temperature(TT)  will be set to 24. "TT = 24" will be displayed on the LCD and anytime 
the rotary encoder is adjusted the new TT will be displated. 

Once the TT is set Current Temperature(CT) and Speed(S) will be displayed in this format "T = 24 S = 683". The fan speed will adjust
accordingly to reach the TT. 

Note that the TT will be kept despite changing modes.
	
# Open Loop
Here the Fan Speed is controlled directly through the rotary encoder.



# Slow Speed
Fan moves slowly a
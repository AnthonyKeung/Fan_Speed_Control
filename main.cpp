#include "mbed.h"
#include <chrono>
#include <cmath>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"
#include "TemperatureSensor.h"
#include "FanControl.h"
#include "TextLCD.h"

#include "PID.h"
#include "TacoProcessing.h"
#define REFRESH_RATE     50ms
#define LED2 PC_0
#define LED3 PB_7

#define PIDRATE 0.1 

DigitalOut biLED(PA_15);
BusOut leds(LED3,LED2);
TemperatureSensor TempSense(24);
TextLCD lcd(PB_15, PB_14, PB_10, PA_8, PB_2, PB_1);
PID controller(10, 0, 0, PIDRATE);
bool enc_rotated = false;      // rotary encoder was rotated left or right
int pulseCount;
int targetTemperature = 24;

//Printing setup 
BufferedSerial mypc(USBTX, USBRX);
FILE* mypcFile1 = fdopen(&mypc, "r+");


int main()
{
    biLED = 0;
    leds.write(0x0);
    enable_Button();
    enableRotaryEncoder();
    setPeriodms(5);

    //Analog input from 0.0 t0 40 degrees
    controller.setInputLimits(0, 40);
    //Pwm output from 0.0 to 1.0
    controller.setOutputLimits(-1, 0);
    //If there's a bias.
    controller.setBias(0.3);
    controller.setMode(1);

    TempSense.checkSensorConnected();

    
    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        leds.write(getMode() + 1);  
        TempSense.read();
        int RPM = RPMcalculate();

        if (getMode() == CLOSEDLOOP)
        {
            if (getRotEncRotated())
            {
                if (getPulseCount() > 0 && getPulseCount() < 40)
                {
                    targetTemperature = getPulseCount();
                }
                else if (getPulseCount() >= 40) 
                {
                    targetTemperature = 40;
                }
                else 
                {
                    targetTemperature = 0;
                }
                controller.setSetPoint(targetTemperature);
                lcd.cls();
                lcd.printf("TT = %d ", targetTemperature);
                ThisThread::sleep_for(1s);
                fprintf(mypcFile1,"The desired Temperature is %d \n" , targetTemperature);
            }
            else
            {
                controller.setProcessValue(TempSense.getTemperatureReading());
                setFan(-1*controller.compute());

                //Display the values 
                lcd.cls();
                lcd.printf("T = %d ", TempSense.getTemperatureReading());
                lcd.locate(0, 1);
                lcd.printf("S= %d", RPM);
                printf("PWM value %f" , -1*controller.compute());
                fprintf(mypcFile1,"The current Temperature is %d \n" ,TempSense.getTemperatureReading());
            }
        }
        else if (getMode() == OPENLOOP)
        {
            if (getPrevMode() != getMode())
            {
                setRotEncResolution(1);
                setRotEncMin(20);
                setRotEncMax(100);
                setRotEncSetPoint(60);
            }
            setFan(getPulseCount() /100);
            printf ("RPM:  %i\n\r" , RPM);
            lcd.cls();
            lcd.printf("S= %d", RPMread());
            lcd.locate(0, 1);
            lcd.printf("DC= %d", int(getPulseCount()));
        }
        else if (getMode() == SLOWCONTROL)
        {
            if (getPrevMode() != getMode())
            {
                setRotEncResolution(0.1);
                setRotEncMin(3.8);
                setRotEncMax(20);
                setRotEncSetPoint(8);
                setFan(1);
                ThisThread::sleep_for(200ms);
            }
            setFan(getPulseCount() /100);
            printf ("RPM:  %i\n\r" , RPM);
            lcd.cls();
            lcd.printf("S= %d", RPMread());
            lcd.locate(0, 1);
            lcd.printf("DC= %.1f", getPulseCount());
        }

        // shaft has been rotated?
        if (getRotEncRotated()) 
        {
            setRotEncRotated(false);
            printf ("Pulses is: %.2f\n\r", getPulseCount());
        } 

        setPrevMode(getMode());
    
    }
}

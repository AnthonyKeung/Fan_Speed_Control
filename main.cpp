#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"
#include "TemperatureSensor.h"
#include "FanControl.h"
#include "Tachometer.h"

#define REFRESH_RATE     1000ms
#define LED2 PC_0
 
BusOut leds(LED1,LED2);
TemperatureSensor TempSense(24);
int pulseCount;

//Printing setup 
BufferedSerial mypc(USBTX, USBRX);
FILE* mypcFile1 = fdopen(&mypc, "r+");



int main()
{
    leds.write(0x0);
    enable_Button();
    enableRotaryEncoder();
    ENABLETACO();
    setPeriodms(5);

    if (TempSense.checkSensorConnected())
    {
        fprintf(mypcFile1, "The sensor is succesfully connected \n");
        wait_us(1000000);
    }
    else
    {
        fprintf(mypcFile1, "The sensor is succesfully connected \n");
        wait_us(1000000);
    }

    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        leds.write(getMode() + 1);   

        if (getMode() == CLOSEDLOOP)
        {
            TempSense.read();
            fprintf(mypcFile1,"The current Temperature is %d \n" ,TempSense.getTemperatureReading());
        }
        else if (getMode() == OPENLOOP)
        {
            setFan(float(getPulseCount()) /100);
            printf ("Revs: %i\n\r" , getRevs(false));
            printf ("RPM:  %i\n\r" , getRPM(true));
        }

        // shaft has been rotated?
        if (getRotEncRotated()) 
        {
            setRotEncRotated(false);
            printf ("Pulses is: %i\n\r", getPulseCount());
        }           
    }
}

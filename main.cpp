#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"
#include "TemperatureSensor.h"

#define REFRESH_RATE     100ms
#define LED2 PC_0
 
BusOut leds(LED1,LED2);
mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);
TemperatureSensor TempSense(24);
bool enc_rotated = false;      // rotary encoder was rotated left or right
int pulseCount;

//Printing setup 
BufferedSerial mypc(USBTX, USBRX);
FILE* mypcFile1 = fdopen(&mypc, "r+");

//interrup-Handler for rotary-encoder rotation
void trigger_rotated() 
{
    enc_rotated = true;               // just set the flag, rest is done outside isr
}


int main()
{
    leds.write(0x0);
    enable_Button();
    REnc.attachROT(&trigger_rotated);

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
        // shaft has been rotated?
        if (enc_rotated) 
        {
            enc_rotated = false;
            pulseCount = REnc.Get();
            printf ("Pulses is: %i\n\r", pulseCount);
        }           
    }
}

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
#include "tach.h"

#define REFRESH_RATE     50ms
#define LED2 PC_0
#define BUF_SIZE    40

BusOut leds(LED1,LED2);
TemperatureSensor TempSense(24);
TextLCD lcd(PB_15, PB_14, PB_10, PA_8, PB_2, PB_1);
bool enc_rotated = false;      // rotary encoder was rotated left or right
int pulseCount;
int RPMcutoff;
int arrayRPM[BUF_SIZE] = {0,0,0,0,0};
int arrayStart = 0;
int sum = 0;
Tach tacho(PA_0,4);

//Printing setup 
BufferedSerial mypc(USBTX, USBRX);
FILE* mypcFile1 = fdopen(&mypc, "r+");

int main()
{
    leds.write(0x0);
    enable_Button();
    enableRotaryEncoder();
    setPeriodms(5);

    if (TempSense.checkSensorConnected())
    {
        fprintf(mypcFile1, "The sensor is succesfully connected \n");
        wait_us(1000000);
    }
    else
    {
        fprintf(mypcFile1, "The sensor is not succesfully connected \n");
        wait_us(1000000);
    }

    int RPM = 0;
    int tempRPM = 0;
    int calibratedRPM = 0;
    
    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        leds.write(getMode() + 1);

        //Removal of high frequency taco pulses
        RPMcutoff = 40 * getPulseCount();
        tempRPM = tacho.getSpeed() * 60;
        while (tempRPM > RPMcutoff)
        {
            tempRPM = tempRPM / 2;
        }
        if (tempRPM < 800)
        {
            tempRPM = ((1-(float(tempRPM)/800)) * 0.35 + 1) * tempRPM;
        }
        arrayRPM[arrayStart] = tempRPM;
        sum = 0;
        for (int i=0; i<BUF_SIZE; i++)
        {
            sum += arrayRPM[i];
        }
        RPM = sum / BUF_SIZE;        

        if (getMode() == CLOSEDLOOP)
        {
            TempSense.read();
            lcd.cls();
            lcd.printf("T = %d ", TempSense.getTemperatureReading());
            lcd.locate(0, 1);
            lcd.printf("S= %d", tacho.getCount());
            fprintf(mypcFile1,"The current Temperature is %d \n" ,TempSense.getTemperatureReading());
        }
        else if (getMode() == OPENLOOP)
        {
            setFan(float(getPulseCount()) /100);
            printf ("RPM:  %i\n\r" , RPM);
        }

        // shaft has been rotated?
        if (getRotEncRotated()) 
        {
            setRotEncRotated(false);
            printf ("Pulses is: %i\n\r", getPulseCount());
        }     

        arrayStart++;
        arrayStart = arrayStart % BUF_SIZE;      
    }
}

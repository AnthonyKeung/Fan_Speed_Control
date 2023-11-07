#include "mbed.h"

#include "TemperatureSensor.h"

enum Mode
{
    CLOSEDLOOP = 0,
    OPENLOOP = 1,
    SLOWCONTROL = 2
};

Mode mode = CLOSEDLOOP;
DigitalOut led1(LED1);
DigitalOut led2(PC_0);
InterruptIn button(BUTTON1);
TemperatureSensor TempSense(24);

void BUTTONINTERUPT()
{
    if (mode < 2)
    {
        mode = static_cast<Mode>(mode + 1);
    }
    else
    {
        mode = CLOSEDLOOP;
    }
    wait_us(5000);
}


int main()
{
    //Printing setup 
    BufferedSerial mypc(USBTX, USBRX);
    FILE* mypcFile1 = fdopen(&mypc, "r+");

    led1 = 0;
    led2 = 0;

    //Interrupt setups
    button.rise(&BUTTONINTERUPT);

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

    wait_us(1000000);

    while (true) 
    {
        TempSense.read();
        fprintf(mypcFile1,"The current Temperature is %d \n" ,TempSense.getTemperatureReading());

        if (mode == CLOSEDLOOP)
        {
            led1 = 1;
            led2 = 0;
        }
        else if (mode == OPENLOOP)
        {
            led1 = 0;
            led2 = 1;
        }
        else if (mode == SLOWCONTROL)
        {
            led1 = 1;
            led2 = 1;
        }
        else 
        {
            printf("Should never reach here");
        }
    }
}

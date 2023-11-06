
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
    led1 = 0;
    led2 = 0;
    button.rise(&BUTTONINTERUPT);
    TempSense.checkSensorConnected();
    while (true) 
    {
        TempSense.read();
        printf("The current Temperature is %d" ,TempSense.getTemperatureReading());
        printf("The mode is %d \n" , mode);
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
    }
}


#include "mbed.h"

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
    wait_us(2000);
}

int main()
{
    led1 = 0;
    led2 = 0;
    button.rise(&BUTTONINTERUPT);
    
    while (true) 
    {
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

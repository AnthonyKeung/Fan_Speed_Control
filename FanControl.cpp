#include "mbed.h"

PwmOut fan(PB_0);

void setFan(float dutyCycle)
{
    fan = dutyCycle;
}

int getFan()
{
    return fan.read();
}
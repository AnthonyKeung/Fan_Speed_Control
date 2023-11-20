#include "mbed.h"
#include <chrono>
#include <ratio>

PwmOut fan(PB_0);

void setPeriodms(int pulse_period_ms = 20)
{
    fan.period_ms(pulse_period_ms);
}

void setFan(float dutyCycle)
{
    fan = dutyCycle;
}

int getFan()
{
    return fan.read();
}
#include "FanControl.h"
#include "mbed.h"
#include "Interrupts.h"
#include <chrono>
#include <ratio>

const chrono::microseconds PWMPERIOD = 100us;

Ticker PWMHigh;
Ticker PWMLow;
Timeout PWMOffset;

void enablePWMsignal(float DutyCycle)
{//const std::chrono::duration<long, std::micro> int_usec = int_ms;
    //std::chrono::duration<int, std::micro> 
    auto pulsePeriod = (DutyCycle * PWMPERIOD);
    if (DutyCycle > 0)
    {
        PWMHigh.attach(&PINSTATUSHIGH,PWMPERIOD);
        if (DutyCycle < 1)
        {
            PWMOffset.attach_us(&offsetLow,pulsePeriod.count());
        }
    }
    else
    {
        PINSTATUSLOW();
    }    
}

void offsetLow()
{
    PWMLow.attach(&PINSTATUSLOW,PWMPERIOD);
}

void setPulsePeriod(float DutyCycle)
{
    PWMHigh.detach();   
    PWMLow.detach();
    PWMOffset.detach(); 
    enablePWMsignal(DutyCycle);
}

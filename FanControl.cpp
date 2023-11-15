#include "FanControl.h"
#include "mbed.h"
#include "Interrupts.h"
#include <chrono>
#include <cmath>
#include <ratio>
#include "platform/mbed_critical.h"


chrono::microseconds PWMPERIOD = 10000us;
chrono::microseconds pulsePeriod = PWMPERIOD;

Ticker PWMHigh;
Ticker PWMLow;
Timeout PWMOffset;

void offsetLow()
{
    PWMLow.attach(&PINSTATUSLOW,PWMPERIOD);
}

void setPulsePeriod(int DutyCycle)
{
    PWMHigh.detach();   
    PWMLow.detach();
    PWMOffset.detach(); 
    PWMPERIOD = std::chrono::microseconds( int(1000000/pow(DutyCycle,1.5)) );
    /*
    if (DutyCycle < 10)
    {
        PWMPERIOD = std::chrono::microseconds(100000/DutyCycle);
    }
    else if (DutyCycle < 20)
    {
        PWMPERIOD = std::chrono::microseconds( 250000);
    }
    else if (DutyCycle < 30)
    {
        PWMPERIOD = std::chrono::microseconds( 100000);
    }
    else if (DutyCycle < 40)
    {
        PWMPERIOD = std::chrono::microseconds(  25000);
    }
    else
    {
        PWMPERIOD = std::chrono::microseconds(   1000);
    }*/


    pulsePeriod = std::chrono::microseconds(PWMPERIOD.count() * DutyCycle / 100) - 60us;

    if (DutyCycle > 0)
    {
        PWMHigh.attach(&PINSTATUSHIGH,PWMPERIOD);
        if (DutyCycle < 100)
        {
            PWMOffset.attach(&offsetLow,pulsePeriod);
        }
    }
    else
    {
        PINSTATUSLOW();
    }  
}

int getPP()
{
    return pulsePeriod.count();
}
int getPwmP()
{
    return PWMPERIOD.count();
}

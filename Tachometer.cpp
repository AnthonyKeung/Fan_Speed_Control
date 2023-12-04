#include "mbed.h"
#include "Tachometer.h"

Timer timer;

int revolutions = 0;
//InterruptIn taco(PA_0);
InterruptIn taco(PB_4);

void TACOINTERRUPT()
{
    revolutions++;   
}

void ENABLETACO()
{
    taco.rise(&TACOINTERRUPT);
    timer.start();
}

int getRevs(bool reset = true)
{
    int tempRevs = revolutions;
    if (reset)
    {
        revolutions = 0;
        timer.reset();
    }
    return tempRevs;
}

int getRPM(bool reset = true)
{
    float elapsedTime =  (float(1000000) / 2 * 60) / static_cast<float>(timer.elapsed_time().count()) ; //2 ticks per rev, 1000000 / 4, * 60 secs
    //float elapsedTime = (1/static_cast<float>(timer.elapsed_time().count())) * 60 / 2 / 1000000 ; //60 secs, 2 ticks a rev, 1000000 us per s (value is inverse of elapsed time)
    int RPM = int(float(revolutions) * elapsedTime); // 
    if (reset)
    {
        revolutions = 0;
        timer.reset();
    }
    return RPM;
}
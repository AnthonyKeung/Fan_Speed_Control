#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = CLOSEDLOOP;

InterruptIn button(BUTTON1);
InterruptIn taco(PA_0);
Timeout debounce_Button;
Timer timer;

int revolutions = 0;

//---------------------------------------BUTTON INTERRUPTS---------------------------------------

void BUTTONINTERRUPT()
{
    button.rise(NULL);

    mode = Mode (fmod((int(mode) + 1), 3));
    
    debounce_Button.attach(&ENABLEBUTTON, DEBOUNCE_TIMER);
}

void ENABLEBUTTON() // define function called in BUTTONINTERRUPT
{
    button.rise(&BUTTONINTERRUPT);
    debounce_Button.detach();
}

//---------------------------------------TACHOMETER INTERRUPTS---------------------------------------

void TACOINTERRUPT() // define function called in BUTTONINTERRUPT
{
    revolutions++;
}

void ENABLETACO() // define function called in BUTTONINTERRUPT
{
    taco.rise(&TACOINTERRUPT);
}

//---------------------------------------Subroutines---------------------------------------

void startTimer()
{
    timer.start();
}

//---------------------------------------GETTERS---------------------------------------

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
    float elapsedTime = 250000 / static_cast<float>(timer.elapsed_time().count()) ; //4 ticks per rev, 1000000 / 4
    int RPM = int(float(revolutions * 60 * elapsedTime)); // 60 seconds per minute, 
    if (reset)
    {
        revolutions = 0;
        timer.reset();
    }
    return RPM;
}

int getMode()
{
    return int(mode);
}

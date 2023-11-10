#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = CLOSEDLOOP;

InterruptIn button(BUTTON1);
InterruptIn taco(PA_0);
Timeout debounce_Button;

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

//---------------------------------------GETTERS---------------------------------------

int getRevs(bool reset = true)
{   
    int tempRevs = revolutions;
    if (reset)
    {
        revolutions = 0;
    }
    return tempRevs;
}

int getMode()
{
    return int(mode);
}

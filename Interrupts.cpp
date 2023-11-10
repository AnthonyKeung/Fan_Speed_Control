#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = CLOSEDLOOP;

InterruptIn button(BUTTON1);
Timeout debounce_Button;

//---------------------------------------BUTTON INTERRUPTS---------------------------------------

void BUTTONINTERRUPT()
{
    button.rise(NULL);

    mode = Mode (fmod((int(mode) + 1), 3));
    
    debounce_Button.attach(&enable_Button, DEBOUNCE_TIMER);
}

void enable_Button() // define function called in BUTTONINTERRUPT
{
    button.rise(&BUTTONINTERRUPT);
    debounce_Button.detach();
}

//---------------------------------------GETTERS---------------------------------------


int getMode()
{
    return int(mode);
}

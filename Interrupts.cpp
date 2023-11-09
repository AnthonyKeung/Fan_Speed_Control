#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#include <math.h>

#define DEBOUNCE_TIMER     20ms

Mode mode = CLOSEDLOOP;
InterruptIn button(BUTTON1);
Timeout debounce_Switch;

void enable_Switch(); // declare function prototype to reference in BUTTONINTERRUPT

void BUTTONINTERRUPT()
{
    mode = Mode (fmod((int(mode) + 1), 3));

    button.rise(NULL);
    debounce_Switch.attach(&enable_Switch, DEBOUNCE_TIMER);
}

void enable_Switch() // define function called in BUTTONINTERRUPT
{
    button.rise(&BUTTONINTERRUPT);
    debounce_Switch.detach();
}

int getMode()
{
    return int(mode);
}

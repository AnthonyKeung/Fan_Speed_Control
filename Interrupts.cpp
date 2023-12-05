#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = OPENLOOP;

InterruptIn button(BUTTON1);
Timeout debounce_Button;

mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);
bool encRotated = false;      // rotary encoder was rotated left or right

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
//---------------------------------------ROTARY ENCODER INTERRUPTS---------------------------------------

void ROTENCINTERRUPT()
{
    encRotated = true;               // just set the flag, rest is done outside isr
}

void enableRotaryEncoder() // define function called in BUTTONINTERRUPT
{
    REnc.attachROT(&ROTENCINTERRUPT);
}
//---------------------------------------GETTERS---------------------------------------

bool getRotEncRotated()
{
    return encRotated;
}

void setRotEncRotated(bool newVal)
{
    encRotated = newVal;
}

int getPulseCount()
{
    if (REnc.Get() > 200)
    {
        REnc.Set(200);
    }
    else if (REnc.Get() < 0) {
    REnc.Set(0);
    }
    
    return (REnc.Get() / 2);
}

int getMode()
{
    return int(mode);
}

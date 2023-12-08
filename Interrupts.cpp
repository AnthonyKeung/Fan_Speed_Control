#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = CLOSEDLOOP;
Mode prevMode = CLOSEDLOOP;

InterruptIn button(BUTTON1);
Timeout debounce_Button;

mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);

bool encRotated = false;      // rotary encoder was rotated left or right
bool modeChanged = false;
int RotEncMin = 0;
int RotEncMax = 200;
float RotEncRes = 1;

//---------------------------------------BUTTON INTERRUPTS---------------------------------------

void BUTTONINTERRUPT()
{
    button.rise(NULL);

    mode = Mode (fmod((int(mode) + 1), 3));
    modeChanged = true;
    
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
    REnc.Set((RotEncMax-RotEncMin)/2);
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

void setRotEncSetPoint(float setPoint)
{
    REnc.Set( int(2 * setPoint / RotEncRes) );
}

void setRotEncMax(float max)
{
    RotEncMax = 2 * max / RotEncRes;
    REnc.Set((RotEncMax-RotEncMin)/2);
}

void setRotEncMin(float min)
{
    RotEncMin = 2 * min / RotEncRes;
    REnc.Set((RotEncMax-RotEncMin)/2);
}

void setRotEncResolution(float res)
{
    RotEncRes = res;
}

float getPulseCount()
{
    if (REnc.Get() > RotEncMax)
    {
        REnc.Set(RotEncMax);
    }
    else if (REnc.Get() < RotEncMin) {
        REnc.Set(RotEncMin);
    }
    
    return (float(REnc.Get())  * RotEncRes / 2); //2 pulses per tick
}

int getMode()
{
    return int(mode);
}

bool getModeChanged()
{
    return modeChanged;
}

void resetModeChange()
{
    modeChanged = false;
}

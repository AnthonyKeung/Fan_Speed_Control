#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#include <math.h>

#define DEBOUNCE_TIMER     15ms
#define MAX     10

Mode mode = CLOSEDLOOP;
int pos = 0;
int prevState = 0b11;

InterruptIn button(BUTTON1);
Timeout debounce_Button;

InterruptIn rotEncA(PA_1);
InterruptIn rotEncB(PA_4);

//---------------------------------------BUTTON INTERRUPTS---------------------------------------

void BUTTONINTERRUPT()
{
    mode = Mode (fmod((int(mode) + 1), 3));

    button.rise(NULL);
    debounce_Button.attach(&enable_Button, DEBOUNCE_TIMER);
}

void enable_Button() // define function called in BUTTONINTERRUPT
{
    button.rise(&BUTTONINTERRUPT);
    debounce_Button.detach();
}

//---------------------------------------ROTARY ENCODER INTERRUPTS---------------------------------------
//----------A RISE----------
/*
void ROTENCINTERRUPT_ARISE()
{
    if (prevState == 0b00)
    {
        pos++;
    }
    else if (prevState == 0b01)
    {
        pos--;
    };
    //pos = pos < 0 ? 0 : pos;
    //pos = pos > MAX ? MAX : pos;
    prevState = prevState || 0b10;
}

void enable_RotEnc_ARISE() 
{
    rotEncA.rise(&ROTENCINTERRUPT_ARISE);
}

//----------A FALL----------
void ROTENCINTERRUPT_AFALL()
{    
    if (prevState == 0b11)
    {
        pos++;
    }
    else if (prevState == 0b10)
    {
        pos--;
    };

    prevState = not(not(prevState) || 0b10);
}

void enable_RotEnc_AFALL() 
{
    rotEncA.fall(&ROTENCINTERRUPT_AFALL);
}

//----------B RISE----------
void ROTENCINTERRUPT_BRISE()
{
    if (prevState == 0b10)
    {
        pos++;
    }
    else if (prevState == 0b00)
    {
        pos--;
    };
    
    prevState = prevState || 0b01;
}

void enable_RotEnc_BRISE()
{
    rotEncB.rise(&ROTENCINTERRUPT_BRISE);
}

//----------B FALL----------
void ROTENCINTERRUPT_BFALL()
{
    if (prevState == 0b01)
    {
        pos++;
    }
    else if (prevState == 0b11)
    {
        pos--;
    };
    

    prevState = not(not(prevState) || 0b01);
}

void enable_RotEnc_BFALL() 
{
    rotEncB.fall(&ROTENCINTERRUPT_BFALL);
}
*/










//---------------------------------------GETTERS---------------------------------------

int getPos()
{
    return pos;
}

int getMode()
{
    return int(mode);
}

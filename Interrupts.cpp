#include "mbed.h"

#include "Interrupts.h"
#include "EnumDef.h"

#define DEBOUNCE_TIMER     100ms

Mode mode = CLOSEDLOOP;

InterruptIn button(BUTTON1);
InterruptIn taco(PA_0);
InterruptIn tacoPWM(PA_9);
Timeout debounce_Button;
Timer timer;

int revolutions = 0;
int totalRevolutions = 0;
float dutyCycle = 0;

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

void PWMTACOHIGH()
{
    revolutions=0;
};

void PWMTACOLOW()
{
    if (dutyCycle != 0)
    {
        totalRevolutions = totalRevolutions + float(revolutions)/dutyCycle;
    }
    revolutions = 0;
};

void ENABLEPWM()
{
    tacoPWM.rise(&PWMTACOHIGH);
    tacoPWM.fall(&PWMTACOLOW);
    timer.start();
};

//---------------------------------------Subroutines---------------------------------------

void dutyCycleUpdate(float dcIn)
{
    dutyCycle = dcIn;
}

//---------------------------------------GETTERS---------------------------------------

int getRevs(bool reset = true)
{   
    PWMTACOLOW();
    int tempRevs = totalRevolutions;
    if (reset)
    {
        revolutions = 0;
        totalRevolutions = 0;
        timer.reset();
    }
    return tempRevs;
}

int getRPM(bool reset = true)
{
    PWMTACOLOW();
    float elapsedTime =  15000000 / static_cast<float>(timer.elapsed_time().count()) ; //4 ticks per rev, 1000000 / 4
    int RPM = int(totalRevolutions * elapsedTime); // 
    if (reset)
    {
        revolutions = 0;
        totalRevolutions = 0;
        timer.reset();
    }
    return RPM;
}

int getMode()
{
    return int(mode);
}

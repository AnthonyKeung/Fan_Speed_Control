#include "mbed.h"
#include "Interrupts.h"
#include "EnumDef.h"

#define DEBOUNCE_TIMER     100ms

InterruptIn button(BUTTON1);
InterruptIn taco(PA_0);
InterruptIn tacoPWM(PA_9);
DigitalOut PWMpin(PB_0);

Timeout debounce_Button;
Timer timer;
bool revCountEnable = false;

Mode mode = CLOSEDLOOP;
int revolutions = 0;
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
    if (revCountEnable)
    {
        revolutions++;
    }    
}

void ENABLETACO() // define function called in BUTTONINTERRUPT
{
    taco.rise(&TACOINTERRUPT);
}

void PWMTACOHIGH()
{
    timer.start();
    revCountEnable = true;
};

void PWMTACOLOW()
{
    timer.stop();
    revCountEnable = false;
};

void ENABLEPWM()
{
    tacoPWM.rise(&PWMTACOHIGH);
    tacoPWM.fall(&PWMTACOLOW);
};

//---------------------------------------FAN PWM INTERRUPTS---------------------------------------

void PINSTATUSHIGH()
{
    PWMpin = 1;
}

void PINSTATUSLOW()
{
    PWMpin = 0;
}

//---------------------------------------Subroutines---------------------------------------

void dutyCycleUpdate(float dcIn)
{
    dutyCycle = dcIn;
}

//---------------------------------------GETTERS---------------------------------------

int getRevs(bool reset = true)
{   
    //PWMTACOLOW();
    int tempRevs = revolutions;
    if (reset)
    {
        revolutions = 0;
        timer.stop();
        timer.reset();
    }
    return tempRevs;
}

int getRPM(bool reset = true)
{
    //PWMTACOLOW();
    float elapsedTime =  30000000 / static_cast<float>(timer.elapsed_time().count()) ; //2 ticks per rev, 1000000 / 4, * 60 secs
    int RPM = int(revolutions * elapsedTime); // 
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

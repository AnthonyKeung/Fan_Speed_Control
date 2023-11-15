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
    if (true)//(revCountEnable)
    {
        revolutions++;
    }    
}

void ENABLETACO() // define function called in BUTTONINTERRUPT
{
    taco.fall(&TACOINTERRUPT);
}

void PWMTACOHIGH()
{
    //timer.start();
    revCountEnable = true;
};

void PWMTACOLOW()
{
    //timer.stop();
    revCountEnable = false;
};

void ENABLEPWM()
{
    tacoPWM.rise(&PWMTACOHIGH);
    tacoPWM.fall(&PWMTACOLOW);
    timer.start();
};

//---------------------------------------FAN PWM INTERRUPTS---------------------------------------

void PINSTATUSHIGH()
{
    core_util_critical_section_enter();
    PWMpin = 1;
    core_util_critical_section_exit();
}

void PINSTATUSLOW()
{
    core_util_critical_section_enter();
    PWMpin = 0;
    core_util_critical_section_exit();}

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
        //timer.stop();
        timer.reset();
    }
    return tempRevs;
}

int getRPM(bool reset = true)
{
    //float elapsedTime =  (float(1000000) / 2 * 60) / static_cast<float>(timer.elapsed_time().count()) ; //2 ticks per rev, 1000000 / 4, * 60 secs
    float elapsedTime = static_cast<float>(timer.elapsed_time().count()) * 60 / 2 / 1000000 ; //60 secs, 2 ticks a rev, 1000000 us per s
    int RPM = int(float(revolutions) * elapsedTime); // 
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

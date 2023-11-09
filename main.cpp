
#include "mbed.h"
#include <chrono>
#include <cstdint>

#define REFRESH_RATE     50ms
#define DEBOUNCE_TIMER     10ms
#define LED2 PC_0

enum Mode : int
{
    CLOSEDLOOP = 0,
    OPENLOOP = 1,
    SLOWCONTROL = 2
};

Mode mode = CLOSEDLOOP;
BusOut leds(LED1,LED2);
InterruptIn button(BUTTON1);
Timeout debounce_Switch;

void enable_Switch(); // declare function to reference in BUTTONINTERRUPT

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

int main()
{
    leds.write(0x0);
    button.rise(&BUTTONINTERRUPT);
    
    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        
        leds.write(int(mode) + 1);         
    }
}

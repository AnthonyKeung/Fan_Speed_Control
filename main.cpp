
#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"

#define REFRESH_RATE     50ms
#define LED2 PC_0

BusOut leds(LED1,LED2);

int main()
{
    leds.write(0x0);
    enable_Switch();

    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        
        leds.write(getMode() + 1);         
    }
}

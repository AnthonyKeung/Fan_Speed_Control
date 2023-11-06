
#include "mbed.h"

DigitalOut led(LED1);

int main()
{
    while (true) 
    {
        led = !led;
        wait_us(500);
    }
}

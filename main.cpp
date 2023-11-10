
#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"

#define REFRESH_RATE     100ms
#define LED2 PC_0

BufferedSerial mypc(USBTX, USBRX); 
BusOut leds(LED1,LED2);
PwmOut FanPWM(PB_0);
mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);
bool enc_rotated = false;      // rotary encoder was rotated left or right
int pulseCount;

//interrup-Handler for rotary-encoder rotation
void trigger_rotated() 
{
    enc_rotated = true;               // just set the flag, rest is done outside isr
}

int main()
{
    leds.write(0x0);
    ENABLEBUTTON();
    ENABLETACO();
    REnc.attachROT(&trigger_rotated);

    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        
        leds.write(getMode() + 1);   
      
        // shaft has been rotated?
        if (enc_rotated) 
        {
            enc_rotated = false;
            pulseCount = REnc.Get();
            printf ("Pulses is: %i\n\r", pulseCount);
        }
        
        printf ("Pulses is: %i\n\r", getRevs(true));

        if (getMode() == 0)    
        {
            FanPWM = 1.0;
        }  
        else
        {
            FanPWM = 0.0;
        }     
    }
}

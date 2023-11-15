
#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <type_traits>

#include "Interrupts.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"
#include "FanControl.h"

#define REFRESH_RATE     1000ms
#define LED2 PC_0

BufferedSerial mypc(USBTX, USBRX);
BusOut leds(LED1,LED2);
mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);

bool enc_rotated = false;      // rotary encoder was rotated left or right
int pulseCount = 0;
int DutyCycle = 100;
int prevDutyCycle = 0;

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
    ENABLEPWM();
    REnc.attachROT(&trigger_rotated);
    REnc.Set(50);
    pulseCount = REnc.Get();
    setPulsePeriod(0);    

    while (true) 
    {
        ThisThread::sleep_for(REFRESH_RATE);
        dutyCycleUpdate(DutyCycle); 
        
        leds.write(getMode() + 1);   
      
        // shaft has been rotated?
        if (enc_rotated) 
        {
            enc_rotated = false;
            pulseCount = REnc.Get();
            printf ("Pulses is: %i\n\r", pulseCount);
        }
        
        
        printf ("Pulse Period in us: %i\n\r", getPP());
        printf ("Pulse Period in us: %i\n\r", getPwmP());
        printf ("Rot Enc is at: %i\n\r", pulseCount);
        printf ("Revs is: %i\n\r", getRevs(false));
        printf ("RPM is: %i\n\r", getRPM(true));

        if (getMode() == 0)    
        {
            DutyCycle = 100;
        }  
        else if (getMode() == 1)    
        {
            DutyCycle = pulseCount;
        }  
        else
        {
            DutyCycle = 0;
        }    

        if (DutyCycle != prevDutyCycle)
        {
            setPulsePeriod(DutyCycle); 
            prevDutyCycle = DutyCycle;
        }
    }
}

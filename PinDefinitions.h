#include "mbed.h"
#include "EnumDef.h"
#include "RotaryEncoder.h"

#define LED2 PC_0

BufferedSerial mypc(USBTX, USBRX);
BusOut leds(LED1,LED2);
mRotaryEncoder REnc(PA_1, PA_4, PC_1, PullUp, 1500);

#include "TacoProcessing.h"
#include "Tach.h"
#include "Interrupts.h"

#define BUF_SIZE    40

Tach tacho(PA_0,4);
int RPMcutoff;
int arrayRPM[BUF_SIZE] = {0,0,0,0,0};
int arrayStart = 0;
int sum = 0;
int RPM = 0;
int tempRPM = 0;

int RPMread()
{
    return RPM;
}

int RPMcalculate()
{

    //Removal of high frequency taco pulses
    RPMcutoff = 40 * getPulseCount();
    tempRPM = tacho.getSpeed() * 60;
    while (tempRPM > RPMcutoff)
    {
        tempRPM = tempRPM / 2;
    }
    //linearization algorithm
    tempRPM = tempRPM^2 / 2050;

    arrayRPM[arrayStart] = tempRPM;
    sum = 0;
    for (int i=0; i<BUF_SIZE; i++)
    {
        sum += arrayRPM[i];
    }
    RPM = sum / BUF_SIZE; 

    arrayStart++;
    arrayStart = arrayStart % BUF_SIZE;
    return RPM;
}

bool getf()
{
    return tacho.getfall();
}

void setf()
{
    tacho.resetfall();
}
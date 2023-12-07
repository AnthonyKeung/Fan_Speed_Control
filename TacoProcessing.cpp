#include "TacoProcessing.h"
#include "Tach.h"
#include "Interrupts.h"

#define BUF_SIZE    40
#define FALL_BUF_SIZE    10

Tach tacho(PA_0,4);
int RPMcutoff;
int arrayRPM[BUF_SIZE] = {0};
bool arrayFall[FALL_BUF_SIZE] = {0};
int arrayRPMHead = 0;
int arrayFallHead = 0;
int sum = 0;
int RPM = 0;
int tempRPM = 0;

int RPMread()
{
    return RPM;
}

int RPMcalculate()
{
    arrayFall[arrayFallHead] = getf();
    arrayFallHead++;
    arrayFallHead = arrayFallHead % FALL_BUF_SIZE;

    bool spinning = false;
    for (int i=0; i<FALL_BUF_SIZE; i++)
    {
        if (arrayFall[i] == true)
        {
            spinning = true;
        }
    }
    

    //Removal of high frequency taco pulses
    RPMcutoff = 40 * getPulseCount();
    if (spinning)
    {
        tempRPM = tacho.getSpeed() * 60;    
        while (tempRPM > RPMcutoff)
        {
            tempRPM = tempRPM / 2;
        }
        //linearization algorithm
        tempRPM = tempRPM^2 / 2050;
    }
    else
    {
        tempRPM = 0;
    }


    arrayRPM[arrayRPMHead] = tempRPM;
    sum = 0;
    for (int j=0; j<BUF_SIZE; j++)
    {
        sum += arrayRPM[j];
    }
    RPM = sum / BUF_SIZE; 

    arrayRPMHead++;
    arrayRPMHead = arrayRPMHead % BUF_SIZE;
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
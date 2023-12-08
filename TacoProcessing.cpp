#include "TacoProcessing.h"
#include "Tach.h"
#include "Interrupts.h"
#include <cstdio>

#define TEMP_RPM_BUF_SIZE    20
#define RPM_BUF_SIZE    10
#define FALL_TEMP_RPM_BUF_SIZE    10

Tach tacho(PA_0,4);
int RPMcutoff;
int arrayTempRPM[TEMP_RPM_BUF_SIZE] = {0};
int arrayRPM[RPM_BUF_SIZE] = {0};
bool arrayFall[FALL_TEMP_RPM_BUF_SIZE] = {0};
int arrayTempRPMHead = 0;
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
    setf();
    arrayFallHead++;
    arrayFallHead = arrayFallHead % FALL_TEMP_RPM_BUF_SIZE;

    bool spinning = false;
    for (int i=0; i<FALL_TEMP_RPM_BUF_SIZE; i++)
    {
        if (arrayFall[i] == true)
        {
            spinning = true;
        }
    }
    

    //Removal of high frequency taco pulses
    // RPMcutoff = 40 * getPulseCount();
    // RPMcutoff = 900 * log_a_to_base_b(getPulseCount(), 5);
    RPMcutoff = 2500 * sqrt(getDC()/100-0.035);
    if (spinning)
    {        
        tempRPM = tacho.getSpeed() * 60;    
        while (tempRPM > RPMcutoff)
        {
            tempRPM = tempRPM / 2;
        }
        // linearization algorithm
        // tempRPM = tempRPM^2 / 2050;
    }
    else
    {
        tempRPM = 0;
    }

    arrayTempRPM[arrayTempRPMHead] = tempRPM;
    
    sum = 0;
    // bool varying = false;    
    for (int j=0; j<TEMP_RPM_BUF_SIZE; j++)
    {
        sum += arrayTempRPM[j];
        // if ((arrayTempRPM[j] != arrayTempRPM[j-1]) and j!=0)
        // {
        //     varying = true;
        // }
    }

    // if (!varying)
    // {
    //     arrayTempRPM[arrayTempRPMHead] = 0;
    //     sum = 0;
    //     for (int j=0; j<TEMP_RPM_BUF_SIZE; j++)
    //     {
    //         sum += arrayTempRPM[j];
    //     }
    // }

    RPM = sum / TEMP_RPM_BUF_SIZE; 

    arrayTempRPMHead++;
    arrayTempRPMHead = arrayTempRPMHead % TEMP_RPM_BUF_SIZE;
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
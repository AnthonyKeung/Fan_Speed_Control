#include "mbed.h"

class TemperatureSensor
{
    private:
    int desiredTemperature;
    int currentTemperature;
    const int TemperatureSensorAddress = 0x9A;
    bool debug_mode = true;

    public:
    TemperatureSensor(int desiredTemperature);
    bool checkSensorConnected();
    void read();
    int getTemperatureReading();
};

TemperatureSensor::TemperatureSensor(int desiredTemperature):
    desiredTemperature(desiredTemperature)
    {
    };

void TemperatureSensor::read()
{
    I2C i2c(PB_9, PB_8);
    char data[2];
    i2c.read(TemperatureSensorAddress, data, 1); 
    currentTemperature = data[0];
    wait_us(100000); 
}

int TemperatureSensor::getTemperatureReading()
{
    return currentTemperature;
}

bool TemperatureSensor::checkSensorConnected()
{
    I2C i2c(PB_9, PB_8);
    if (!i2c.write(TemperatureSensorAddress, NULL, 0)) 
    { // 0 returned is ok
        return true;
    }
    return false;

}

enum Mode
{
    CLOSEDLOOP = 0,
    OPENLOOP = 1,
    SLOWCONTROL = 2
};

Mode mode = CLOSEDLOOP;
DigitalOut led1(LED1);
DigitalOut led2(PC_0);
InterruptIn button(BUTTON1);
TemperatureSensor TempSense(24);

void BUTTONINTERUPT()
{
    if (mode < 2)
    {
        mode = static_cast<Mode>(mode + 1);
    }
    else
    {
        mode = CLOSEDLOOP;
    }
    wait_us(5000);
}


int main()
{
    //Printing setup 
    BufferedSerial mypc(USBTX, USBRX);
    FILE* mypcFile1 = fdopen(&mypc, "r+");

    led1 = 0;
    led2 = 0;

    //Interrupt setups
    button.rise(&BUTTONINTERUPT);

    if (TempSense.checkSensorConnected())
    {
        fprintf(mypcFile1, "The sensor is succesfully connected \n");
        wait_us(1000000);
    }
    else
    {
        fprintf(mypcFile1, "The sensor is succesfully connected \n");
        wait_us(1000000);
    }

    wait_us(1000000);

    while (true) 
    {
        TempSense.read();
        fprintf(mypcFile1,"The current Temperature is %d \n" ,TempSense.getTemperatureReading());

        if (mode == CLOSEDLOOP)
        {
            led1 = 1;
            led2 = 0;
        }
        else if (mode == OPENLOOP)
        {
            led1 = 0;
            led2 = 1;
        }
        else if (mode == SLOWCONTROL)
        {
            led1 = 1;
            led2 = 1;
        }
        else 
        {
            printf("Should never reach here");
        }
    }
}

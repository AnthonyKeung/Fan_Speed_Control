#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(int desiredTemperature):
    desiredTemperature_p(desiredTemperature)
    {
    };

void TemperatureSensor::read()
{
    I2C i2c(PB_8, PB_9);
    char data[2];
    i2c.read(TemperatureSensorAddress, data, 1); 
    currentTemperature_p = data[0];
    if (debug_mode == true)
    {
        BufferedSerial mypc(USBTX, USBRX);
        FILE* mypcFile1 = fdopen(&mypc, "r+");
        fprintf(mypcFile1,"Device with address 0x%x with\r\n", TemperatureSensorAddress); 
        fprintf(mypcFile1,"Temperature is  :: %d\n\r", data[0]);
    }
}

int TemperatureSensor::getTemperatureReading()
{
    return currentTemperature_p;
}

bool TemperatureSensor::checkSensorConnected()
{
    I2C i2c(PB_8, PB_9);
    for (int address=0; address<256; address+=2) 
    {
        if (!i2c.write(address, NULL, 0)) 
        { // 0 returned is ok
            return true;
        }
    }
    return false;

}

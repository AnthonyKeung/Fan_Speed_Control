#include "TemperatureSensor.h"
#include "mbed.h"

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
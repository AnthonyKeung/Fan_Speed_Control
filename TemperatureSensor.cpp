#include "TemperatureSensor.h"
#include "mbed.h"
#include "TextLCD.h"

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
}

int TemperatureSensor::getTemperatureReading()
{
    return currentTemperature;
}

bool TemperatureSensor::checkSensorConnected()
{
    I2C i2c(PB_9, PB_8);
    TextLCD lcd(PB_15, PB_14, PB_10, PA_8, PB_2, PB_1);
    if (!i2c.write(TemperatureSensorAddress, NULL, 0)) 
    { // 0 returned is ok
        lcd.cls();
        lcd.printf("Succesful");
        return true;
    }
    else
    {
        lcd.printf("Fail");
        return false;
    }
    

}
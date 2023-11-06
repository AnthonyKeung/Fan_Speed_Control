#include "mbed.h"

class TemperatureSensor
{
    private:
    int desiredTemperature_p;
    int currentTemperature_p;
    const int TemperatureSensorAddress = 0x9A;
    bool debug_mode = true;

    public:
    TemperatureSensor(int desiredTemperature);
    bool checkSensorConnected();
    void read();
    int getTemperatureReading();
};
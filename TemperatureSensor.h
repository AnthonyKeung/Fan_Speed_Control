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

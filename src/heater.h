#ifndef HEATER_H
#define HEATER_H

#include "settings.h"
#include "temp.h"

class Heater
{
public:
    Heater(Settings *settings, Temp *temp);
    void begin();
    void update();
    bool heating();

private:
    bool _heating = false;
    float _thresholdDelta = 0.5;
    int _fanStopIterationsCount = 0;
    const int _fanStopDelay = 60; //loop iterations
    const int _switchFan = 14;
    const int _switchHeater = 12;
    Settings *_settings;
    Temp *_temp;
    void heaterFanOn();
    void heaterFanOff();
    void heaterOff();
    void delayedFanOff();
};

#endif
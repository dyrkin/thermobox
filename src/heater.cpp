#include "heater.h"

Heater::Heater(Settings *settings, Temp *temp)
{
    _settings = settings;
    _temp = temp;
}

void Heater::begin()
{
    pinMode(_switchFan, OUTPUT);
    pinMode(_switchHeater, OUTPUT);
}

void Heater::update()
{
    if (_settings->heaterEnabled())
    {
        if (_temp->getAvgTemp() < _settings->temperatureThreshold() - _thresholdDelta)
        {
            heaterFanOn();
        }
        else if (_temp->getAvgTemp() >= _settings->temperatureThreshold())
        {
            heaterOff();
        }
        if (!_heating)
        {
            delayedFanOff();
        }
    }
    else
    {
        heaterFanOff();
    }
}

void Heater::heaterFanOn()
{
    _heating = true;
    _fanStopIterationsCount = 0;
    digitalWrite(_switchFan, LOW);
    digitalWrite(_switchHeater, LOW);
}

void Heater::heaterFanOff()
{
    _heating = false;
    _fanStopIterationsCount = 0;
    digitalWrite(_switchFan, HIGH);
    digitalWrite(_switchHeater, HIGH);
}

void Heater::heaterOff()
{
    _heating = false;
    digitalWrite(_switchHeater, HIGH);
}

void Heater::delayedFanOff()
{
    if (_fanStopIterationsCount >= _fanStopDelay)
    {
        digitalWrite(_switchFan, HIGH);
    }
    _fanStopIterationsCount++;
}
#include "temp.h"

Temp::Temp(int avgInterval) : _tempSensorMcp(),
                              _tempSensorSht(),
                              _twoSensorAvgTemp(avgInterval) {}

void Temp::begin()
{
    _tempSensorMcp.begin(0x18);
    _tempSensorMcp.setResolution(3);
    _tempSensorMcp.wake();

    _tempSensorSht.begin(0x44);

    _twoSensorAvgTemp.begin();
}

void Temp::update()
{
    float t = _tempSensorMcp.readTempC();
    if (!isnan(t))
    {
        _mcpTemp = t;
    }

    t = _tempSensorSht.readTemperature();
    if (!isnan(t))
    {
        _shtTemp = t;
    }
    float h = _tempSensorSht.readHumidity();
    if (!isnan(h))
    {
        _shtHumidity = h;
    }

    _twoSensorAvgTemp.reading(_mcpTemp);
    _twoSensorAvgTemp.reading(_shtTemp);
}

float Temp::getMcpTemp()
{
    return _mcpTemp;
}
float Temp::getShtTemp()
{
    return _shtTemp;
}
float Temp::getAvgTemp()
{
    return _twoSensorAvgTemp.getAvg();
}
float Temp::getShtHumidity()
{
    return _shtHumidity;
}
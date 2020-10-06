#ifndef METRICS_H
#define METRICS_H

#include "settings.h"
#include "temp.h"
#include "heater.h"
#include <InfluxDbClient.h>

class Metrics
{
public:
    Metrics(Settings *settings, Temp *temp, Heater *heater);
    void begin();
    void send();

private:
    Settings *_settings;
    Temp *_temp;
    Heater *_heater;
    InfluxDBClient *_client;
};

#endif
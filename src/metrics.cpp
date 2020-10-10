#include "metrics.h"
#include "heater.h"

Metrics::Metrics(Settings *settings, Temp *temp, Heater *heater)
{
    _settings = settings;
    _temp = temp;
    _heater = heater;
}

void Metrics::begin()
{
    _client = new InfluxDBClient{_settings->influxdbUrl(), _settings->influxdbDatabaseName()};
}

void Metrics::send()
{
    if(_client->getServerUrl() != _settings->influxdbUrl()) {
        _client = new InfluxDBClient{_settings->influxdbUrl(), _settings->influxdbDatabaseName()};
    }
    if (_settings->influxdbUrl())
    {
        Point pointDevice("thermobox");
        pointDevice.addTag("kind", "sensor");
        pointDevice.addField("mcp_temp", _temp->getMcpTemp());
        pointDevice.addField("sht_temp", _temp->getShtTemp());
        pointDevice.addField("two_sensor_avg_temp", _temp->getAvgTemp());
        pointDevice.addField("sht_humidity", _temp->getShtHumidity());
        pointDevice.addField("heating", _heater->heating());
        pointDevice.addField("threshold", _settings->temperatureThreshold());

        _client->writePoint(pointDevice);
    }
}
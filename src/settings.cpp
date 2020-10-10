#include "settings.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

#define WIFI_SSID_KEY "wifi_ssid"
#define WIFI_PASSWORD_KEY "wifi_password"
#define INFLUXDB_URL_KEY "inlfuxdb_url"
#define INFLUXDB_DATABASE_NAME_KEY "influxdb_db_name"
#define TEMPERATURE_THRESHOLD "temperature_threshold"
#define HEATER_ENABLED "heater_enabled"

bool Settings::read()
{
    File configFile = LittleFS.open("/settings.json", "r");
    if (!configFile)
        write();

    if (!configFile)
    {
        Serial.println("Failed to open settings file");
        return false;
    }

    size_t size = configFile.size();

    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println("Failed to parse settings file");
        return false;
    }

    _ssid = strdup(doc[WIFI_SSID_KEY]);
    _password = strdup(doc[WIFI_PASSWORD_KEY]);
    _influxdbUrl = strdup(doc[INFLUXDB_URL_KEY]);
    _influxdbDatabaseName = strdup(doc[INFLUXDB_DATABASE_NAME_KEY]);

    _temperatureThreshold = doc[TEMPERATURE_THRESHOLD];
    _heaterEnabled = doc[HEATER_ENABLED];

    return true;
}

bool Settings::write()
{
    StaticJsonDocument<200> doc;
    doc[WIFI_SSID_KEY] = _ssid;
    doc[WIFI_PASSWORD_KEY] = _password;
    doc[INFLUXDB_URL_KEY] = _influxdbUrl;
    doc[INFLUXDB_DATABASE_NAME_KEY] = _influxdbDatabaseName;
    doc[TEMPERATURE_THRESHOLD] = _temperatureThreshold;
    doc[HEATER_ENABLED] = _heaterEnabled;

    File configFile = LittleFS.open("/settings.json", "w");
    if (!configFile)
    {
        Serial.println("Failed to open settings file for writing");
        return false;
    }

    serializeJson(doc, configFile);
    return true;
}

const char *Settings::ssid()
{
    return _ssid;
}

void Settings::setSsid(const char *ssid)
{
    _ssid = strdup(ssid);
}

const char *Settings::password()
{
    return _password;
}

void Settings::setPassword(const char *password)
{
    _password = strdup(password);
}

const char *Settings::influxdbUrl()
{
    return _influxdbUrl;
}

void Settings::setInfluxdbUrl(const char *influxdbUrl)
{
    _influxdbUrl = strdup(influxdbUrl);
}

const char *Settings::influxdbDatabaseName()
{
    return _influxdbDatabaseName;
}

void Settings::setInfluxdbDatabaseName(const char *influxdbDatabaseName)
{
    _influxdbDatabaseName = strdup(influxdbDatabaseName);
}

float Settings::temperatureThreshold()
{
    return _temperatureThreshold;
}

void Settings::setTemperatureThreshold(float temperatureThreshold)
{
    _temperatureThreshold = temperatureThreshold;
}

bool Settings::heaterEnabled()
{
    return _heaterEnabled;
}

void Settings::setHeaterEnabled(bool heaterEnabled)
{
    _heaterEnabled = heaterEnabled;
}
#include "settings.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

#define WIFI_SSID_KEY "wifi_ssid"
#define WIFI_PASSWORD_KEY "wifi_password"
#define INFLUXDB_URL_KEY "inlfuxdb_url"
#define INFLUXDB_DATABASE_NAME_KEY "influxdb_db_name"

Settings::Settings() {}

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

    _ssid = doc[WIFI_SSID_KEY];
    _password = doc[WIFI_PASSWORD_KEY];
    _influxdbUrl = doc[INFLUXDB_URL_KEY];
    _influxdbDatabaseName = doc[INFLUXDB_DATABASE_NAME_KEY];

    return true;
}

bool Settings::write()
{
    StaticJsonDocument<200> doc;
    doc[WIFI_SSID_KEY] = _ssid;
    doc[WIFI_PASSWORD_KEY] = _password;
    doc[INFLUXDB_URL_KEY] = _influxdbUrl;
    doc[INFLUXDB_DATABASE_NAME_KEY] = _influxdbDatabaseName;

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
    _ssid = ssid;
}

const char *Settings::password()
{
    return _password;
}

void Settings::setPassword(const char *password)
{
    _password = password;
}

const char *Settings::influxdbUrl()
{
    return _influxdbUrl;
}

void Settings::setInfluxdbUrl(const char *influxdbUrl)
{
    _influxdbUrl = influxdbUrl;
}

const char *Settings::influxdbDatabaseName()
{
    return _influxdbDatabaseName;
}

void Settings::setInfluxdbDatabaseName(const char *influxdbDatabaseName)
{
    _influxdbDatabaseName = influxdbDatabaseName;
}

float Settings::temperatureThreshold()
{
    return _temperatureThreshold;
}

void Settings::setTemperatureThreshold(float temperatureThreshold)
{
    _temperatureThreshold = temperatureThreshold;
}
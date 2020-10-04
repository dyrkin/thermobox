#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    Settings();
    bool read();
    bool write();
    const char *ssid();
    void setSsid(const char *ssid);
    const char *password();
    void setPassword(const char *password);
    const char *influxdbUrl();
    void setInfluxdbUrl(const char *influxdbUrl);
    const char *influxdbDatabaseName();
    void setInfluxdbDatabaseName(const char *influxdbDatabaseName);
    float temperatureThreshold();
    void setTemperatureThreshold(float temperatureThreshold);

private:
    const char *_ssid;
    const char *_password;
    const char *_influxdbUrl;
    const char *_influxdbDatabaseName;
    float _temperatureThreshold;
};

#endif
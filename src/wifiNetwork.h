#ifndef WIFI_NETWORK_H
#define WIFI_NETWORK_H

#include <Arduino.h>
#include "settings.h"

class WiFiNetwork
{
public:
    WiFiNetwork(Settings *settings);
    void begin();
    String ip();

private:
    Settings *_settings;
    void connect(const char *ssid, const char *password);
    bool softAP;
};

#endif

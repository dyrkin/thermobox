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
};

#include <ESPAsyncWebServer.h>
#include "settings.h"
#include "temp.h"
#include "heater.h"
#include "wifiNetwork.h"

class UI
{
public:
    UI(int port, Settings *settings, Temp *temp, Heater *heater, WiFiNetwork *wifiNetwork);
    void begin();

private:
    int _port;
    AsyncWebServer _server;
    Settings *_settings;
    Temp *_temp;
    Heater *_heater;
    WiFiNetwork *_wifiNetwork;
    void onSettingsUpdated();
};
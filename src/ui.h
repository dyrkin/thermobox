#include <ESPAsyncWebServer.h>
#include "settings.h"

class UI
{
public:
    UI(int port, Settings *settings);
    void begin();

private:
    int _port;
    AsyncWebServer _server;
    Settings *_settings;
};
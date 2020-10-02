#include <ESPAsyncWebServer.h>

class UI
{
public:
    UI(int port);
    void begin();

private:
    int _port;
    AsyncWebServer _server;
};
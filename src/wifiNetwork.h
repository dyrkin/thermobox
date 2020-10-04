#include <Arduino.h>

class WiFiNetwork
{
public:
    WiFiNetwork(const char *ssid, const char *password);
    void begin();
    String ip();

private:
    const char *_ssid;
    const char *_password;
};

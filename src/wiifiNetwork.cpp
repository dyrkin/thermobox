#include "wifiNetwork.h"
#include <ESP8266WiFi.h>

const char *_softAPSsid = "thermobox";
const char *_softAPPassword = "thermobox";

WiFiNetwork::WiFiNetwork(Settings *settings)
{
  _settings = settings;
}

void WiFiNetwork::begin()
{
  connect(_settings->ssid(), _settings->password());
}

void WiFiNetwork::connect(const char *ssid, const char *password)
{
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  if (!ssid || *ssid == 0x00 || strlen(ssid) == 0)
  {
    WiFi.softAP(_softAPSsid, _softAPPassword);
    Serial.println("Created Soft AP");
    softAP = true;
  }
  else
  {
    int attempts = 20;
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && attempts >= 0)
    {
      attempts--;
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Can't connect to WiFi");
      connect(NULL, NULL);
    }
    else
    {
      softAP = false;
      Serial.println("Connected to WiFi");
    }
  }
}

String WiFiNetwork::ip()
{
  if (softAP)
  {
    return WiFi.softAPIP().toString();
  }
  return WiFi.localIP().toString();
}
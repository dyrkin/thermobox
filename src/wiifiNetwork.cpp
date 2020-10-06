#include "wifiNetwork.h"
#include <ESP8266WiFi.h>

WiFiNetwork::WiFiNetwork(Settings *settings)
{
  _settings = settings;
}

void WiFiNetwork::begin()
{
  WiFi.begin(_settings->ssid(), _settings->password());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}

String WiFiNetwork::ip() {
  return WiFi.localIP().toString();
}
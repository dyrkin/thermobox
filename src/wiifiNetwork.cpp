#include "wifiNetwork.h"
#include <ESP8266WiFi.h>

WiFiNetwork::WiFiNetwork(const char *ssid, const char *password)
{
  _ssid = ssid;
  _password = password;
}

void WiFiNetwork::begin()
{
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}

String WiFiNetwork::ip() {
  return WiFi.localIP().toString();
}
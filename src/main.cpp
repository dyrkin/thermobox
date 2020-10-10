#include "ui.h"
#include "temp.h"
#include "wifiNetwork.h"
#include <LittleFS.h>
#include "settings.h"
#include "heater.h"
#include "metrics.h"

Settings settings;
Temp temp{20};
Heater heater{&settings, &temp};
WiFiNetwork wifi{&settings};
UI ui{80, &settings, &temp, &heater, &wifi};
Metrics metrics{&settings, &temp, &heater};

void setup()
{
  Serial.begin(115200);
  delay(1000);
  LittleFS.begin();
  settings.read();
  configTzTime("CET-1CEST", "pool.ntp.org", "time.nis.gov");

  wifi.begin();
  ui.begin();
  temp.begin();
  heater.begin();
  metrics.begin();
  Serial.println(wifi.ip());
}

void loop()
{
  temp.update();
  heater.update();
  metrics.send();
  delay(1000);
}
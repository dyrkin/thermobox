#include <Arduino.h>
#include <Hash.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "Adafruit_SHT31.h"
#include <SPI.h>
#include <InfluxDbClient.h>
#include "movingAvg.h"
#include "ui.h"
#include "temp.h"
#include "secrets.h"
#include "wifiNetwork.h"
#include <LittleFS.h>
#include "settings.h"
#include "heater.h"

//influx
const char *influxdbUrl = "http://192.168.1.26:8086";
const char *influxdbDatabaseName = "iot";

Settings settings;

InfluxDBClient client(influxdbUrl, influxdbDatabaseName);
UI ui(80, &settings);
Temp temp(20);
Heater heater(&settings, &temp);
WiFiNetwork wifi(ssid, password);

String ip = "";
float threshold = 50;
float thresholdDelta = 0.5;
bool heating = false;
const int fanStopDelay = 60; //loop iterations
int fanStopIterationsCount = 0;

void setup()
{
  LittleFS.begin();
  settings.read();
  configTzTime("CET-1CEST", "pool.ntp.org", "time.nis.gov");
  Serial.begin(115200);

  wifi.begin();
  ui.begin();
  temp.begin();

  heater.begin();
  Serial.println(wifi.ip());
}

void sendMeasurementsToInflux()
{
  Point pointDevice("thermobox");
  pointDevice.addTag("kind", "sensor");
  pointDevice.addTag("ip", ip);
  pointDevice.addField("mcp_temp", temp.getMcpTemp());
  pointDevice.addField("sht_temp", temp.getShtTemp());
  pointDevice.addField("two_sensor_avg_temp", temp.getAvgTemp());
  pointDevice.addField("sht_humidity", temp.getShtHumidity());
  pointDevice.addField("heating", heating);

  client.writePoint(pointDevice);
}

void loop()
{
  temp.update();
  sendMeasurementsToInflux();
  heater.update();
  delay(1000);
}
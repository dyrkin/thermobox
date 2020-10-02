#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "Adafruit_SHT31.h"
#include <SPI.h>
#include <InfluxDbClient.h>
#include "movingAvg.h"
#include "ui.h"
#include "temp.h"
#include "wifi_secrets.h"

//influx
const char *influxdbUrl = "http://192.168.1.26:8086";
const char *influxdbDatabaseName = "iot";

//switches
const int switchFan = 14;
const int switchHeater = 12;

InfluxDBClient client(influxdbUrl, influxdbDatabaseName);
UI ui(80);
Temp temp(20);

String ip = "";
float threshold = 50;
float thresholdDelta = 0.5;
bool heating = false;
const int fanStopDelay = 60; //loop iterations
int fanStopIterationsCount = 0;

void setup()
{
  configTzTime("Europe/Warsaw", "pool.ntp.org", "time.nis.gov");

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  ip = WiFi.localIP().toString();

  Serial.println(ip);

  ui.begin();
  temp.begin();

  pinMode(switchFan, OUTPUT);
  pinMode(switchHeater, OUTPUT);
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

void onOffHeater()
{
  if (temp.getAvgTemp() < threshold - thresholdDelta)
  {
    heating = true;
    fanStopIterationsCount = 0;
    digitalWrite(switchFan, LOW);
    digitalWrite(switchHeater, LOW);
  }
  else if (temp.getAvgTemp() >= threshold)
  {
    heating = false;
    if (fanStopIterationsCount >= fanStopDelay)
    {
      digitalWrite(switchFan, HIGH);
    }
    digitalWrite(switchHeater, HIGH);
    fanStopIterationsCount++;
  }
  else if (!heating)
  {
    fanStopIterationsCount++;
  }
}

void loop()
{
  temp.update();
  sendMeasurementsToInflux();
  onOffHeater();
  delay(1000);
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "Adafruit_SHT31.h"
#include <SPI.h>
#include <InfluxDbClient.h>
#include "movingAvg.h"

//influx
const char *influxdbUrl = "http://192.168.1.26:8086";
const char *influxdbDatabaseName = "iot";

//wifi
const char *ssid = "ssid";
const char *password = "pwd";

//switches
const int switchFan = 14;
const int switchHeater = 12;

AsyncWebServer server(80);
InfluxDBClient client(influxdbUrl, influxdbDatabaseName);

Adafruit_MCP9808 tempSensorMcp9808 = Adafruit_MCP9808();
Adafruit_SHT31 tempSensorSht31 = Adafruit_SHT31();
movingAvg twoSensorAvgTemp(20);

float mcpTemp = 0;
float shtTemp = 0;
float humidity = 0;
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
  LittleFS.begin();

  tempSensorMcp9808.begin(0x18);
  tempSensorMcp9808.setResolution(3);
  tempSensorMcp9808.wake();

  tempSensorSht31.begin(0x44);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  ip = WiFi.localIP().toString();

  Serial.println(ip);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  twoSensorAvgTemp.begin();

  pinMode(switchFan, OUTPUT);
  pinMode(switchHeater, OUTPUT);
}

void readSensorData()
{
  float t = tempSensorMcp9808.readTempC();
  if (!isnan(t))
  {
    mcpTemp = t;
  }

  t = tempSensorSht31.readTemperature();
  if (!isnan(t))
  {
    shtTemp = t;
  }
  float h = tempSensorSht31.readHumidity();
  if (!isnan(h))
  {
    humidity = h;
  }

  twoSensorAvgTemp.reading(mcpTemp);
  twoSensorAvgTemp.reading(shtTemp);

  Serial.print("mcp: ");
  Serial.println(mcpTemp);
  Serial.print("sht: ");
  Serial.println(shtTemp);
  Serial.print("avg: ");
  Serial.println(twoSensorAvgTemp.getAvg());
}

void sendMeasurementsToInflux()
{
  Point pointDevice("thermobox");
  pointDevice.addTag("kind", "sensor");
  pointDevice.addTag("ip", ip);
  pointDevice.addField("mcp_temp", mcpTemp);
  pointDevice.addField("sht_temp", shtTemp);
  pointDevice.addField("two_sensor_avg_temp", twoSensorAvgTemp.getAvg());
  pointDevice.addField("humidity", humidity);
  pointDevice.addField("heating", heating);

  client.writePoint(pointDevice);
}

void onOffHeater()
{
  if (twoSensorAvgTemp.getAvg() < threshold - thresholdDelta)
  {
    heating = true;
    fanStopIterationsCount = 0;
    digitalWrite(switchFan, LOW);
    digitalWrite(switchHeater, LOW);
  }
  else if (twoSensorAvgTemp.getAvg() >= threshold)
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
  readSensorData();
  sendMeasurementsToInflux();
  onOffHeater();
  delay(1000);
}
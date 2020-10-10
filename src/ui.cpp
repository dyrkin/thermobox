#include <LittleFS.h>
#include "ui.h"
#include <SPI.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <Ticker.h>
#include "functional"

UI::UI(int port, Settings *settings, Temp *temp, Heater *heater, WiFiNetwork *wifiNetwork) : _server(port)
{
    _settings = settings;
    _temp = temp;
    _heater = heater;
    _wifiNetwork = wifiNetwork;
}

void UI::begin()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html");
    });

    _server.serveStatic("/css/", LittleFS, "/css/");
    _server.serveStatic("/js/", LittleFS, "/js/");

    _server.on("/settings", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(200);
        doc["ssid"] = _settings->ssid();
        doc["influxdb_url"] = _settings->influxdbUrl();
        doc["influxdb_db_name"] = _settings->influxdbDatabaseName();
        doc["heater_enabled"] = _settings->heaterEnabled();

        serializeJson(doc, *response);

        request->send(response);
    });

    _server.on(
        "/settings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            DynamicJsonDocument doc(200);
            auto error = deserializeJson(doc, ((const char *)data));

            if (error)
            {
                Serial.println("Failed to parse config file");
            }

            _settings->setSsid(doc["ssid"]);
            _settings->setPassword(doc["password"]);
            _settings->setInfluxdbUrl(doc["influxdb_url"]);
            _settings->setInfluxdbDatabaseName(doc["influxdb_db_name"]);
            _settings->setHeaterEnabled(doc["heater_enabled"]);
            request->send(200);
            // Ticker tick;
            // tick.once_ms_scheduled(1, [&]() {
            //     _wifiNetwork->begin();
            // });
        });

    _server.on("/state", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(200);
        doc["heating"] = _heater->heating();
        doc["avg_temp"] = _temp->getAvgTemp();
        doc["mcp_temp"] = _temp->getMcpTemp();
        doc["sht_temp"] = _temp->getShtTemp();
        doc["sht_humidity"] = _temp->getShtHumidity();
        doc["heater_enabled"] = _settings->heaterEnabled();

        serializeJson(doc, *response);

        request->send(response);
    });

    _server.begin();
}

void UI::onSettingsUpdated()
{
    _wifiNetwork->begin();
}
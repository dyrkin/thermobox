#include <LittleFS.h>
#include "ui.h"
#include <SPI.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "settings.h"

UI::UI(int port, Settings *settings) : _server(port) {
    _settings = settings;
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

        serializeJson(doc, *response);

        request->send(response);
    });

    _server.on("/state", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(200);
        doc["ssid"] = _settings->ssid();
        doc["influxdb_url"] = _settings->influxdbUrl();
        doc["influxdb_db_name"] = _settings->influxdbDatabaseName();

        serializeJson(doc, *response);

        request->send(response);
    });

    _server.begin();
}
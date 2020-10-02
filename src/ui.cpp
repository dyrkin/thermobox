#include <LittleFS.h>
#include "ui.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

UI::UI(int port) : _server(port) {}

void UI::begin()
{
    LittleFS.begin();
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html");
    });

    _server.serveStatic("/css/", LittleFS, "/css/");
    _server.serveStatic("/js/", LittleFS, "/js/");

    _server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse *response = new AsyncJsonResponse();
        JsonObject &root = response->getRoot();
        root["key1"] = "key number one";
        JsonObject &nested = root.createNestedObject("nested");
        nested["key1"] = "key number one";

        response->setLength();
        request->send(response);
    });

    _server.begin();
}
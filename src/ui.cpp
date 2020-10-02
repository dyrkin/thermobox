#include <LittleFS.h>
#include "ui.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

UI::UI(int port)
{
    _port = port;
}

void UI::begin()
{
    LittleFS.begin();
    AsyncWebServer server(_port);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html");
    });

    server.serveStatic("/css/", LittleFS, "/css/");
    server.serveStatic("/js/", LittleFS, "/js/");

    server.begin();
}
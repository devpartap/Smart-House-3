#include "ota.hpp"

#include "definations.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


void setupOTA()
{
    CLOG_LN("\n=== SETTING UP OTA ===");

    ArduinoOTA.setPort(OTA_PORT);
    
#ifdef _DEBUG_

    ArduinoOTA.onStart([]() {
        
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) 
        {
            type = "sketch";
        } 
        else 
        {  
            // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        CLOG_LN("Start updating " + type);

    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        CONSOLE(printf("Progress: %u%%\r", (progress / (total / 100))));
    });

    ArduinoOTA.onEnd([]() {
        CLOG_LN("\nEnd");
    });

    ArduinoOTA.onError([](ota_error_t error) {

        CONSOLE(printf("Error[%u]: ", error));
        if (error == OTA_AUTH_ERROR) 
        {
            CLOG_LN("Auth Failed");
        } 
        else if (error == OTA_BEGIN_ERROR) 
        {
            CLOG_LN("Begin Failed");
        } 
        else if (error == OTA_CONNECT_ERROR) 
        {
            CLOG_LN("Connect Failed");
        } 
        else if (error == OTA_RECEIVE_ERROR) 
        {
            CLOG_LN("Receive Failed");
        } 
        else if (error == OTA_END_ERROR) 
        {
            CLOG_LN("End Failed");
        }
    });

#endif

    ArduinoOTA.begin();
    CLOG_LN("Done");
}

void handleOTA()
{
    ArduinoOTA.handle();
}

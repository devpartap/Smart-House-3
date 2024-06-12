#define _DEBUG_

#include "definations.h"
#include "communication.h"

void setup()
{

    CONSOLE(begin(115200));
    CONSOLE(setDebugOutput(true));

    connectToWiFi();
    delay(50);

    connectToMaster();

    CLOG("Setup Complete!");
}

void loop()
{

    // WiFiClient client = server.accept();

    // if (client.connected())
    // {
    //     CLOG_LN("Connected to client");
    // }

    // client.stop();
}
#include "definations.hpp"

#include "esp8266.hpp"
#include "requesthandling.hpp"


void setup()
{
    CONSOLE(begin(115200));

    espStart();
    espSetup();
    espConnectAP();

    CLOG("\n === Setup Complete! === \n\n");
}

void loop()
{
    espReadToPrimaryBuffer();
    processRequest();
    pingActiveConnections();
}

#define _DEBUG_

#include "definations.hpp"
#include "communication.hpp"
#include "devicecontrol.hpp"
#include "ota.hpp"

void setup()
{
    CONSOLE(begin(115200));
    CONSOLE(setDebugOutput(true));
    
    connectToWiFi();

    setupEEPROM();
    setupSwitches();
    setupOTA();

    reportToMaster();
    startServer();

    CLOG("\n === Setup Complete! === \n\n");
    
}

void loop()
{
    listenForMaster();
    listenSwitchChange(); 
    
    handleOTA();
}

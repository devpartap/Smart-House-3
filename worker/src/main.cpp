#define _DEBUG_

#include "definations.hpp"
#include "communication.hpp"

void setup()
{
    randomSeed(analogRead(RANDOM_SEED_PIN));

    CONSOLE(begin(115200));
    CONSOLE(setDebugOutput(true));

    connectToWiFi();

#ifdef _DEBUG_
    uint16 max_arrange_time = random(100,MAX_ARRANGE_TIME);
    CLOG("Wait : ");
    CLOG_LN(max_arrange_time);

    delay(random(100,max_arrange_time));
#else
    delay(random(100,MAX_ARRANGE_TIME));
#endif
    connectToMaster();
    startServer();
    
    CLOG("Setup Complete!");

}

void loop()
{
    listenForMaster();
}
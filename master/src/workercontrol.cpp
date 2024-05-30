#include "definations.h"

#include "esp8266.h"

#include "workercontrol.h"
#include "workerslayout.h"

void registerStatus();

void wkr_toggleDevice(const uint8_t* deviceID, const bool stateToChange)
{
    // send message
    uint16_t roomindex;
    if(deviceID[0] == 0)
        roomindex = deviceID[1];
    else
        roomindex = floorMap[deviceID[0] - 1] + deviceID[1] - 1;

    // espconnectSend(RoomMap[roomindex].BoardList[deviceID[3] - 1].ip,stateToChange);

    
}
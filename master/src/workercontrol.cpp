#include "definations.hpp"
#include "esp8266.hpp"

#include "workercontrol.hpp"
#include "workerslayout.hpp"


void registerStatus();

void workerToggleDevice(const uint8_t *_device_id, const bool _state)
{
    // send message
    uint16_t roomindex;
    if (_device_id[0] == 0)
        roomindex = _device_id[1];
    else
        roomindex = floorMap[_device_id[0] - 1] + _device_id[1] - 1;

    // espconnectSend(RoomMap[roomindex].BoardList[_device_id[3] - 1].ip,_state);
}
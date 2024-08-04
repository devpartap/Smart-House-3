#pragma once
#include <Arduino.h>

// --------- Type Defs ---------
enum DeviceType : int8_t
{ // max types 127
    Light = 1,
    Led_Strip,
    Fan,
    Power_Socket

};

// --------- DATA STRUCTURES ---------
struct WorkerDS
{
    unsigned char ip[8];
    bool active;

    int8_t *device_list;
    const uint8_t no_of_devices;

    WorkerDS(int8_t *_device_list,const uint8_t _no_of_devices)
            :active(false), device_list(_device_list),no_of_devices(_no_of_devices)
    {
        CLOG_LN("after this");
        CLOG_LN(no_of_devices);
    }
};

struct RoomDS
{
    WorkerDS *board_list; 
    uint8_t no_of_boards;

    RoomDS(WorkerDS *_board_list,uint8_t numbers)
            : board_list(_board_list), no_of_boards(numbers)
    {
        CLOG_LN(no_of_boards);
    }
};

#define ACTIVEROOM(x,y) &room_map[((x == 0) ? y : floor_map[x - 1][0] + y)]
#define ACTIVEBOARD(x,y,z) &room_map[((x == 0) ? y : floor_map[x - 1][0] + y)].board_list[z]


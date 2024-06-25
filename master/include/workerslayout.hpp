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

    WorkerDS(int8_t *_device_list,const uint8_t & _no_of_devices)
            :active(false), device_list(_device_list),no_of_devices(_no_of_devices)
    {
    }
};

struct RoomDS
{
    WorkerDS *board_list;
    uint8_t no_of_boards;

    RoomDS(WorkerDS *_board_list, uint8_t _no_of_boards)
            : board_list(_board_list), no_of_boards(_no_of_boards)
    {
    }
};

// --------- STORAGE ---------

// --- Devices' Layout
int8_t F0_R0_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light)
};

int8_t F1_R0_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Led_Strip),
        -(DeviceType::Led_Strip),
        -(DeviceType::Fan),
        -(DeviceType::Power_Socket)
};

int8_t F1_R1_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Fan),
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Power_Socket),
        -(DeviceType::Fan),
        -(DeviceType::Power_Socket)
};


// --- Boards' Layout
WorkerDS F0_R0_room_map[] = {
        WorkerDS(F0_R0_B0_state,2)
};

WorkerDS F1_R0_room_map[] = {
        WorkerDS(F1_R0_B0_state,7),
        // WorkerDS(5,2)
};

WorkerDS F1_R1_room_map[] = {
        WorkerDS(F1_R1_B0_state,9),
        // WorkerDS(5,2),
        // WorkerDS(7,2)
};



// --------- MAPS ---------

RoomDS room_map[] = {
        RoomDS(F0_R0_room_map,1), //no boards, no devices
        RoomDS(F1_R0_room_map,1),
        RoomDS(F1_R1_room_map,1)

};

const uint16_t floor_map[][2] = { // these numbers should not be 0
        {1,2}, // Room Index Range, No of rooms,
        {3,16}
};

#define ACTIVEROOM(x,y) &room_map[((x == 0) ? y : floor_map[x - 1][0] + y)]
#define ACTIVEBOARD(x,y,z) &room_map[((x == 0) ? y : floor_map[x - 1][0] + y)].board_list[z]
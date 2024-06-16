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
    uint8_t device_iterations_sp;
    uint8_t no_of_devices;

    WorkerDS(const uint8_t &_device_iterations_sp,const uint8_t & _no_of_devices)
            :active(false), device_iterations_sp(_device_iterations_sp),
            no_of_devices(_no_of_devices)
    {
    }
};

struct RoomDS
{
    WorkerDS *board_list;
    int8_t *device_list;
    uint8_t no_of_boards;
    uint16_t no_of_devices;

    RoomDS(WorkerDS *_board_list, int8_t *_device_list, uint8_t _no_of_boards, uint16_t _no_of_devices)
            : board_list(_board_list), device_list(_device_list), no_of_boards(_no_of_boards),
              no_of_devices(_no_of_devices)
    {
    }
};

// --------- STORAGE ---------

// --- Devices' Layout
int8_t F0_R0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light)
};

int8_t F1_R0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Led_Strip),
        -(DeviceType::Led_Strip),
        -(DeviceType::Fan),
        -(DeviceType::Power_Socket)
};

int8_t F1_R1_state[] = {
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
        WorkerDS(0,2)
};

WorkerDS F1_R0_room_map[] = {
        WorkerDS(0,7),
        // WorkerDS(5,2)
};

WorkerDS F1_R1_room_map[] = {
        WorkerDS(0,9),
        // WorkerDS(5,2),
        // WorkerDS(7,2)
};



// --------- MAPS ---------

RoomDS room_map[] = {
        RoomDS(F0_R0_room_map, F0_R0_state, 1,2), //no boards, no devices
        RoomDS(F1_R0_room_map, F1_R0_state, 2,7),
        RoomDS(F1_R1_room_map, F1_R1_state, 3,9)

};

const uint16_t floor_map[][2] = { // these numbers should not be 0
        {1,2}, // Room Index Range, No of rooms, No of Boards, 
        {3,16}
};
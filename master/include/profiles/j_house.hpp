#pragma once
#include "../workerslayout.hpp"

// --- Devices' Layout ---
int8_t F0_R0_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light)
};

int8_t F1_R0_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Led_Strip),
        // -(DeviceType::Led_Strip),
        // -(DeviceType::Fan),
        // -(DeviceType::Power_Socket)
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


// --- Boards' Layout ---
WorkerDS F0_R0_room_map[] = {
        WorkerDS(F0_R0_B0_state,sizeof(F0_R0_B0_state)/sizeof(F0_R0_B0_state[0]))
};

WorkerDS F1_R0_room_map[] = {
        WorkerDS(F1_R0_B0_state,sizeof(F1_R0_B0_state)/sizeof(F1_R0_B0_state[0])),
        // WorkerDS(F1_R0_B0_state,7),
        // WorkerDS(5,2)
};

WorkerDS F1_R1_room_map[] = {
        WorkerDS(F1_R1_B0_state,sizeof(F1_R1_B0_state)/sizeof(F1_R1_B0_state[0])),
        // WorkerDS(5,2),
        // WorkerDS(7,2)
};


// --------- MAPS ---------

RoomDS room_map[] = {
        RoomDS(F0_R0_room_map,sizeof(F0_R0_room_map)/sizeof(F0_R0_room_map[0])), //no boards, no devices
        RoomDS(F1_R0_room_map,sizeof(F1_R0_room_map)/sizeof(F1_R0_room_map[0])),
        RoomDS(F1_R1_room_map,sizeof(F1_R1_room_map)/sizeof(F1_R1_room_map[0]))

};

const uint16_t floor_map[][2] = { // these numbers should not be 0
        {1,2}, // Room Index Range, Devices Index Range
        {3,13}
};
#pragma once
#include "../workerslayout.hpp"

// --- Devices' Layout ---
int8_t F0_R0_B0_state[] = {
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Fan),
        -(DeviceType::Power_Socket)
};


// --- Boards' Layout ---
WorkerDS F0_R0_room_map[] = {
        WorkerDS(F0_R0_B0_state,sizeof(F0_R0_B0_state)/sizeof(F0_R0_B0_state[0])),
};


// --------- MAPS ---------

RoomDS room_map[] = {
        RoomDS(F0_R0_room_map,sizeof(F0_R0_room_map)/sizeof(F0_R0_room_map[0])),
};

const uint16_t floor_map[][2] = { // these numbers should not be 0
        {1,4}, // Room Index Range, Devices Index Range
};
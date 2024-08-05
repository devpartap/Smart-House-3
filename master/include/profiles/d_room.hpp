#pragma once
#include "../workerslayout.hpp"

// --- Devices' Layout ---
int8_t F0_R0_B0_state[] = {
        -(DeviceType::Power_Socket)
        -(DeviceType::Light),
        -(DeviceType::Light),
        -(DeviceType::Fan),
};


// --- Boards' Layout ---
WorkerDS F0_R0_room_map[] = {
        WorkerDS(F0_R0_B0_state,4),
};


// --------- MAPS ---------

RoomDS room_map[] = {
        RoomDS(F0_R0_room_map,1),
};

const uint16_t floor_map[][2] = { // these numbers should not be 0
        {1,4}, // Room Index Range, Devices Index Range
};
#pragma once

#include "definations.hpp"

#define _ACTIVE_BOARD(x,y,z) _ACTIVE_BOARD_FLOORID_ == x && _ACTIVE_BOARD_ROOMID_ == y && _ACTIVE_BOARD_ID_ == z

// --- Devices & State ---

struct Device {
    bool m_relay_state;
    bool m_switch_state;
    const char * m_relay_pin;
    const char * m_switch_pin;

    Device(const char* _relay_pin, const char * _switch_pin,bool _relay_state = false)
        :m_relay_pin(_relay_pin), m_switch_pin(_switch_pin),
         m_relay_state(_relay_state), m_switch_state(false) 
    { }

    Device() = delete;
};

#if _ACTIVE_BOARD(1,0,0)

    Device ConnectedDevices[] = {
        // Device("D0"),
        // Device(false,"D1"),
        // Device(false,"D2"),
        // Device(true,"D3"),
        // Device(true,"D4"),
        // Device(false,"D5"),
        // Device(true,"D6"),
    };
    const uint8_t g_no_of_devices = 7; 

#elif _ACTIVE_BOARD(1,1,0)

    Device ConnectedDevices[] = {
        Device(true,"D0"),
        Device(false,"D1"),
        Device(true,"D2"),
        Device(true,"D3"),
        Device(true,"D4"),
        Device(false,"D5"),
        Device(false,"D6"),
        Device(true,"D7"),
        Device(true,"D8")
    };

    const uint8_t g_no_of_devices = 9; 

#endif
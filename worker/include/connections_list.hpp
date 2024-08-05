#pragma once

#include "definations.hpp"

#define _ACTIVE_BOARD(x,y,z) _ACTIVE_BOARD_FLOORID_ == x && _ACTIVE_BOARD_ROOMID_ == y && _ACTIVE_BOARD_ID_ == z

// --- Devices & State ---

struct Device {

    const uint8_t m_relay_pin;
    const uint8_t m_switch_pin;
    bool m_relay_state;
    bool m_switch_state;

    uint8_t m_eeprom_index;

    Device(const uint8_t _relay_pin, const uint8_t _switch_pin)
        :m_relay_pin(_relay_pin), m_switch_pin(_switch_pin)
    { 
    }

    Device() = delete;
};

#if _ACTIVE_BOARD(0,0,0)

    Device ConnectedDevices[] = {
        Device(D3,D0),
        Device(D5,D1),
        Device(D6,D2),
        Device(D7,D9),
    };

#elif _ACTIVE_BOARD(1,0,0)

    Device ConnectedDevices[] = {
        Device(D5,D0),
        Device(D6,D1),
        Device(D7,D2),
        Device(D8,3), // 3 -> rx
        // Device(true,"D4"),
        // Device(false,"D5"),
        // Device(true,"D6"),
    };

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

#endif

const uint8_t g_no_of_devices = sizeof(ConnectedDevices) / sizeof(Device);
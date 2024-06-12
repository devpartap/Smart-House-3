#pragma once
#include <Arduino.h>

// --- DEBUGGING STUFF ---
#ifdef _DEBUG_

#define CLOG(x) Serial.print(x)
#define CLOG_LN(x) Serial.println(x)
#define CONSOLE(x) Serial.x

#else
#define CLOG(x)
#define CLOG_LN(x)
#define CONSOLE(x)
#endif

// --- CONSTANTS ---

// Board'ss
inline const  uint8_t g_floor_id = 1;
inline const  uint8_t g_room_id = 1;
inline const  uint8_t  g_board_id = 1;

inline const char* g_wifi_ssid = "Harvinder Singh";
inline const char* g_wifi_password = "gurjot2050";





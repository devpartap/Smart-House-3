#pragma once
#include <Arduino.h>

// --- OPTIONS ---

// #define _DEBUG_
#define _NODEMCU2_

#define _ACTIVE_BOARD_FLOORID_ 0
#define _ACTIVE_BOARD_ROOMID_  0
#define _ACTIVE_BOARD_ID_      0

#define RANDOM_SEED_PIN A0
#define MAX_RANDOM_WAIT 1000
#define OTA_PORT 8266
#define SERVER_PORT 8080
#define MASTER_TIMEOUT_INLOOP 400

// --- CONSTANTS ---

// inline char g_wifi_ssid[] = {'B','o','g','a','l','s',' ','W','i','F','i','\0'};
// inline char g_wifi_password[] = {'B','o','g','a','l','s','w','i','f','i','7','\0'};
inline char g_wifi_ssid[] = {'H','a','r','v','i','n','d','e','r',' ','S','i','n','g','h','\0'};
inline char g_wifi_password[] = {'g','u','r','j','o','t','2','0','5','0','\0'};
inline char g_ip[16];

inline const char* master_ip = "192.168.29.167";
inline short master_port = 80;

inline bool master_acknowledged = false;

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


// --- BOARD DEPENDENT STUFF ---

inline const  uint8_t g_floor_id = _ACTIVE_BOARD_FLOORID_;
inline const  uint8_t g_room_id = _ACTIVE_BOARD_ROOMID_;
inline const  uint8_t  g_board_id = _ACTIVE_BOARD_ID_ ;

extern const uint8_t g_no_of_devices; // don't make it zero











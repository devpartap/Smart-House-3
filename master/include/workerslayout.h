#pragma once

// --------- Type Defs ---------
enum DeviceType : int8_t
{ // max types 127
    Light = 1,
    Led_Strip,
    Fan,
    Power_Socket

};
// --------- Structures ---------
struct BoardDS
{
    const char *ip;
    bool active;
    uint16_t device_itterations;

    BoardDS(const char *_ip, const uint16_t &_device_itterations)
        : ip(_ip), active(false), device_itterations(device_itterations)
    {
    }
};

struct RoomMapPtr
{
    BoardDS *BoardList;
    uint8_t *DeviceList;
    uint8_t noBoards;

    RoomMapPtr(BoardDS *_BoardList, uint8_t *_DeviceList, const uint8_t &_noBoards)
        : BoardList(_BoardList), DeviceList(_DeviceList), noBoards(_noBoards)
    {
    }
};

// // --------- Storage ---------

int8_t F0_R1_State[] = {
    -(DeviceType::Light),
    -(DeviceType::Light)};

int8_t F1_R1_State[] = {
    -(DeviceType::Light),
    -(DeviceType::Light),
    -(DeviceType::Light),
    -(DeviceType::Led_Strip),
    -(DeviceType::Led_Strip),
    -(DeviceType::Fan),
    -(DeviceType::Power_Socket)};

int8_t F0_R2_State[] = {
    -(DeviceType::Light),
    -(DeviceType::Light),
    -(DeviceType::Light),
    -(DeviceType::Fan),
    -(DeviceType::Light),
    -(DeviceType::Light),
    -(DeviceType::Power_Socket),
    -(DeviceType::Fan),
    -(DeviceType::Power_Socket)};

BoardDS RoomMap_F0_R1[] = {
    BoardDS("29.101", 1)};

BoardDS RoomMap_F1_R1[] = {
    BoardDS("29.102", 4),
    BoardDS("29.103", 6)};

BoardDS RoomMap_F1_R2[] = {
    BoardDS("29.104", 3),
    BoardDS("29.105", 6),
    BoardDS("29.106", 8)};

// --------- Maps ---------

RoomMapPtr RoomMap[] = {
    RoomMapPtr(RoomMap_F0_R1, F0_R1_State, 1),
    RoomMapPtr(RoomMap_F1_R1, F1_R1_State, 1),
    RoomMapPtr(RoomMap_F1_R2, F0_R2_State, 1)};

uint8_t floorMap[] = {
    1,
    2};

// 1.1 1.2

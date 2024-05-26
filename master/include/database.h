#pragma once

enum deviceType : int8_t {
    Light = 1,
    Led_Strip,
    Fan,
    Power_Socket
    

};


int8_t f0_Room1[] = {
    -(deviceType::Light),
    -(deviceType::Light),
};

uint8_t f0_Room1_ips[] = {
    10,
    15
};

int8_t f1_Room1[] = { 
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Led_Strip),
    -(deviceType::Led_Strip),
    -(deviceType::Fan),
    -(deviceType::Power_Socket)
};

int8_t f1_Room1_ips[] = { 
    20,
    21,
    22,
    23,
    24,
    25,
    26
};

int8_t f1_Room2[] = { 
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Light),
    -(deviceType::Fan),
    -(deviceType::Fan),
    -(deviceType::Power_Socket),
    -(deviceType::Power_Socket)
};
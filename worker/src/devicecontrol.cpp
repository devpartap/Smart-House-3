#include "definations.hpp"

#include "devicecontrol.hpp"
#include "connections_list.hpp"


void compressDevicesState(char * arry)
{
    uint8_t array_size = 1+ (uint8_t)((g_no_of_devices-1)/8);
    arry[array_size] = '\0';

    for(uint8 irr = 0;irr != array_size; irr++)
    {
        arry[irr] = 0b00000000;
    }

    CLOG_LN("--");

    uint8_t tmp;
    for(uint8_t i = 0; i < g_no_of_devices; i++)
    {
        CLOG_LN((uint8_t)arry[0]);
        tmp = ConnectedDevices[i].m_state;
        tmp = tmp << (7-(i%8));
        arry[i/8] +=  tmp;
        
    }


    CLOG("compressed:");
#ifdef _DEBUG_
    for(uint8_t irr = 0;irr != array_size + 1; irr++)
    {
        CLOG((uint8_t)arry[irr]);
        CLOG("-");
    }
    CLOG_LN(' ');

#endif
}

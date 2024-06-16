#include "definations.hpp"

#include "workercontrol.hpp"
#include "workerslayout.hpp"

#include "esp8266.hpp"

void registerWorker(const uint8_t *_worker_data)
{
    uint8_t room_space = (_worker_data[0] == 0) ? _worker_data[1] : floor_map[_worker_data[0] - 1][0] + _worker_data[1];
    WorkerDS *active_board = &room_map[room_space].board_list[_worker_data[2]];

    active_board->active = true;

    uint8_t irr = 3;
    for (; _worker_data[irr] != '-'; irr++)
    {
        active_board->ip[irr - 3] = _worker_data[irr];
    }
    active_board->ip[irr - 3] = '\0';
    irr = irr + 1;

    uint8_t tmp;
    for (uint8_t i = active_board->device_iterations_sp;
         i < active_board->device_iterations_sp + _worker_data[irr];
         i++)
    {
        tmp = _worker_data[irr + 1 + (uint8_t)(i / 8)];
        tmp = tmp << (uint8_t)(i % 8);

        if ((room_map[room_space].device_list[i] & 0b10000000) == (tmp & 0b10000000)) // here the 0 in tmp indicates off but 0 in device list
        {                                                                             // indicates 1 so that's why using '=' instead of  '!='
            room_map[room_space].device_list[i] *= -1;
        }

    }
}

void getFloorStatus(uint8_t floor,char* to_send)
{
    uint16_t space_irr = 0;

     for(uint16_t i = (floor == 0) ? 0 : floor_map[floor - 1][0]; 
                  i < floor_map[floor][0]; i++)
    {
        for(uint8_t j = 0; j < room_map[i].no_of_devices; j++)
        {
            // to_send[space_irr] = ((~room_map[i].device_list[j]) & (uint8_t)0b10000000) >> 7;
            // space_irr++;

            if((room_map[i].device_list[j] & (uint8_t)0b10000000) == (uint8_t)0b10000000)
            {
                to_send[space_irr] = '0';
            }
            else
            {
                to_send[space_irr] = '1';
            }
            space_irr++;
        }
    }

#ifdef _DEBUG_
    space_irr = 0;
    for(;space_irr < floor_map[floor][1];space_irr++)
    {
        CLOG(to_send[space_irr]);
        CLOG('-');
    }
    CLOG_LN(' ');

#endif

}

bool chnageDeviceState(const uint8_t * _worker_data)
{
    // Discrypted char :- U| |1|.|2|.|1|.|7|.|0|
    uint8_t room_space = (_worker_data[0] == 0) ? _worker_data[1] : floor_map[_worker_data[0] - 1][0] + _worker_data[1];
    WorkerDS *active_board = &room_map[room_space].board_list[_worker_data[2]];
    
    if(!active_board->active)
    {
        return false;
    }
    

    const char command[4] = {'T',_worker_data[3],_worker_data[4],'\0'};
    sendWorkerCommand((const char*)active_board->ip,(const char*)command);
    return true;
}


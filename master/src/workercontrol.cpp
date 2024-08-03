#include "definations.hpp"

#include "workercontrol.hpp"
#include "workerslayout.hpp"

#include "esp8266.hpp"

void registerWorker(const uint8_t *_worker_data)
{
    WorkerDS * active_board = ACTIVEBOARD(_worker_data[0],_worker_data[1],_worker_data[2]);

    active_board->active = true;

    uint8_t irr = 3;
    for (; _worker_data[irr] != '-'; irr++)
    {
        active_board->ip[irr - 3] = _worker_data[irr];
    }
    active_board->ip[irr - 3] = '\0';
    irr = irr + 1;

    uint8_t tmp;
    for (uint8_t i = 0; i < _worker_data[irr]; i++)
    {
        tmp = _worker_data[irr + 1 + (uint8_t)(i / 8)];
        tmp = tmp << (uint8_t)(i % 8);

        if ((active_board->device_list[i] & 0b10000000) == (tmp & 0b10000000)) // here the 0 in tmp indicates off but 0 in device list
        {                                                                      // indicates 1 so that's why using '=' instead of  '!='
            active_board->device_list[i] *= -1;
        }

    }

#ifdef _DEBUG_
    for(uint8_t i = 0; i < active_board->no_of_devices; i++)
    {
        CLOG(active_board->device_list[i]);
        CLOG(' ');
    }
    CLOG_LN(' ');

#endif

}

void getFloorStatus(uint8_t floor,char* to_send)
{
    uint16_t space_irr = 0;

     for(uint16_t i = (floor == 0) ? 0 : floor_map[floor - 1][0]; 
                  i < floor_map[floor][0]; i++)
    {
        for(uint8_t j = 0; j < room_map[i].no_of_boards; j++)
        {
            for(uint8_t k = 0; k < room_map[i].board_list[j].no_of_devices; k++)
            {
                if(room_map[i].board_list[j].device_list[k] < 0)
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

void changeDeviceState(const uint8_t * _worker_data)
{
    WorkerDS *active_board = ACTIVEBOARD(_worker_data[0],_worker_data[1],_worker_data[2]);

    if((_worker_data[4]) == ((active_board->device_list[_worker_data[3]] & 0b10000000) >> 7))
    {
        active_board->device_list[_worker_data[3]] *= -1;
    }
}

bool updateWorkerDeviceState(const uint8_t * _worker_data)
{
    // Discrypted char Format :- U| |1|.|2|.|1|.|7|.|0|

    WorkerDS *active_board = ACTIVEBOARD(_worker_data[0],_worker_data[1],_worker_data[2]);

    if(!active_board->active)
    {
        return false;
    }
    
    const uint8_t command[3] = {1,_worker_data[3],_worker_data[4]};
    
    sendWorkerCommand((const char*)active_board->ip,command,3);
    
    if((_worker_data[4]) == ((active_board->device_list[_worker_data[3]] & 0b10000000) >> 7))
    {
        active_board->device_list[_worker_data[3]] *= -1;
    }

    return true;
}


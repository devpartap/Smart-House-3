#include "definations.hpp"
#include "esp8266.hpp"
#include "cryptography.hpp"
#include "workercontrol.hpp"

enum Request : char 
{
    WEBSOCKET_MESSAGE = (char)129,
    WEBSOCKET_CLOSING_HANDSHAKE = (char)136,
    WEBSOCKET_HANDSHAKE = 'G',
    FLOOR_STATUS_REQUEST = 'F',
    UPDATE_DEVICE = 'U',
    WORKER_STATUS ='W',
};


bool websockets_connections[] = {false, false, false, false};

// Function Signatures Below
void decodeWebsocet(char *_recv_msg, char *_buffer, const uint8_t _buffer_size);


void processRequest(const CStrWithSize &_newrequest, const uint16_t &_stIndex = 0)
{
    CLOG_LN("PROCESSING REQUEST!");

    int16_t start_index = CStrWithSize::indexOf(_newrequest, "+IPD", _stIndex);
    CLOG("St:");
    CLOG_LN(start_index);

    // Processing Request Packet ---
    if (start_index != -1)
    {
        unsigned char connection_no = _newrequest[start_index + 5];
        uint16_t request_starting_pt = CStrWithSize::indexOf(_newrequest, ":", start_index + 8);
        uint16_t request_length = 0;

        for (uint16_t i = start_index + 7; i < request_starting_pt; i++)
        {
            request_length *= 10;
            request_length += _newrequest[i] - '0';
        }

        char request_type = _newrequest[request_starting_pt + 1];

        // Logging Request Parameters
        CLOG("cNo:");
        CLOG(connection_no);
        CLOG(", rst:");
        CLOG(request_starting_pt);
        CLOG(", rln:");
        CLOG(request_length);
        CLOG(", rtyp:");
        CLOG_LN(request_type);

        // Websocket Processing 
        if (request_type == Request::WEBSOCKET_MESSAGE)
        {

            char msg[request_length - 6];
            decodeWebsocet(_newrequest.strptr + request_starting_pt + 1, msg, request_length - 6);

            if (msg[0] == Request::FLOOR_STATUS_REQUEST)
            {
                uint8_t floor = msg[1] - '0';
                char to_send_Data[floor_map[floor][1]];
                // to_send_Data[floor_map[floor][1]] = '\0';

                getFloorStatus(floor,to_send_Data);

                // sendDataOnWebSocket(connection_no,(const char*)to_send_Data);
                sendDataOnWebSocket(connection_no,to_send_Data,floor_map[floor][1]);
            }

            else if (msg[0] == Request::UPDATE_DEVICE)
            {
                uint8_t decive_change_data[] = {0, 0, 0, 0, 0};

                uint8_t info_type = 0;
                uint8_t i = 2;
                for (; i < request_length - 7; i++)
                {
                    if (msg[i] == '.')
                    {
                        info_type++;
                    }
                    else
                    {
                        decive_change_data[info_type] *= 10;
                        decive_change_data[info_type] += msg[i] - '0';
                    }
                }

                decive_change_data[1] -= 1;
                decive_change_data[2] -= 1;
                decive_change_data[3] -= 1;
                decive_change_data[4] = msg[i] - '0';
               
                if(chnageDeviceState(decive_change_data))
                {
                     delay(100);
                    CLOG("inhere!");
                    sendDataOnWebSocket(connection_no, "OK");
                }
                else
                {
                     delay(100);
                    CLOG("innothere!");
                    sendDataOnWebSocket(connection_no, "~");
                }

            }
        }

        else if (request_type == Request::WEBSOCKET_HANDSHAKE)
        {
            uint16_t index = CStrWithSize::indexOf(_newrequest, "ket-Key", 300);
            CStrWithSize key(_newrequest.strptr + index + 9, 24);

#ifdef _DEBUG_
            CStrWithSize::print(key);
#endif

            espConnectWebSocket(connection_no, processWebsocketKey(key));
            websockets_connections[connection_no - '0'] = true;
        }
        else if (request_type == Request::WEBSOCKET_CLOSING_HANDSHAKE)
        {
            websockets_connections[connection_no - '0'] = false;
            CLOG("Closed : ");
            CLOG_LN(connection_no - '0');
        }

        // Worker Processing
        else if (request_type == Request::WORKER_STATUS)
        {

#ifdef _DEBUG_
            for(uint8_t j = 0; j < request_length; j++)
            {
                CLOG((uint8_t)_newrequest[request_starting_pt + 1 + j]);
                CLOG('-');
            }
            CLOG_LN(' ');
#endif

            registerWorker((const uint8_t *)(_newrequest.strptr + request_starting_pt + 2));

            espSendChar(connection_no,'k');
            delay(10);
            espClose(connection_no);
                
        }

        if (_newrequest.length - (request_length + start_index) >= 14)
        {
            processRequest(_newrequest, request_length + start_index);
        }
    }
}


void decodeWebsocet(char *_recv_msg, char *_buffer, const uint8_t _buffer_size)
{

    for (uint8_t i = 0; i < _buffer_size; i++)
    {
        _buffer[i] = (_recv_msg[i + 6] ^ _recv_msg[2 + (i % 4)]);
    }

#ifdef _DEBUG_
    CLOG("Discrypted char :- ");
    for (int i = 0; i < _buffer_size; i++)
    {
        CLOG(_buffer[i]);
        CLOG('|');
    }
    CLOG('\n');
#endif

}

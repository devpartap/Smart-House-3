#include "definations.h"
#include "esp8266.h"
#include "cryptography.h"

#define WEBSOCKET_MESSAGE (char)129
#define WEBSOCKET_CLOSING_HANDSHAKE (char)136
#define WEBSOCKET_HANDSHAKE 'G'
#define FLOOR_STATUS_REQUEST 'F'
#define UPDATE_DEVICE 'U'

#define WORKER_STATUS 'W'

 bool websockets_connections[] = {false, false, false, false};


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


void processRequest(const CStrWithSize &_newrequest, const uint16_t &_stIndex = 0)
{
    CLOG_LN("PROCESSING REQUEST!");

    int16_t start_index = CStrWithSize::indexOf(_newrequest, "+IPD", _stIndex);
    CLOG_LN(start_index);

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

        CLOG_LN(connection_no);
        CLOG_LN(request_starting_pt);
        CLOG_LN(request_length);
        CLOG(request_type);
        CLOG(" : ");
        CLOG_LN((uint8_t)_newrequest.strptr[request_starting_pt + 1]);

        if (request_type == WEBSOCKET_MESSAGE)
        {

            char msg[request_length - 6];
            decodeWebsocet(_newrequest.strptr + request_starting_pt + 1, msg, request_length - 6);

            if (msg[0] == FLOOR_STATUS_REQUEST)
            {
                if (msg[1] == '0')
                {
                    sendDataOnWebSocket(connection_no, "01");
                }
                else if (msg[1] == '1')
                {
                    sendDataOnWebSocket(connection_no, "0000011100000001");
                }
            }

            else if (msg[0] == UPDATE_DEVICE)
            {
                uint8_t device_adress[4] = {0, 0, 0, 0};

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
                        device_adress[info_type] *= 10;
                        device_adress[info_type] += msg[i] - '0';
                    }
                }

                bool device_new_state = msg[i] - '0';

                sendDataOnWebSocket(connection_no, "OK");
            }
        }

        else if (request_type == WEBSOCKET_HANDSHAKE)
        {
            uint16_t index = CStrWithSize::indexOf(_newrequest, "ket-Key", 300);
            CStrWithSize key(_newrequest.strptr + index + 9, 24);

#ifdef _DEBUG_
            CStrWithSize::print(key);
#endif
            espConnectWebSocket(connection_no, processWebsocketKey(key));
            websockets_connections[connection_no - '0'] = true;
        }
        else if (request_type == WEBSOCKET_CLOSING_HANDSHAKE)
        {
            websockets_connections[connection_no - '0'] = false;
            CLOG("Closed : ");
            CLOG_LN(connection_no - '0');
        }

        else if (request_type == WORKER_STATUS)
        {
            char a = _newrequest.strptr[request_starting_pt + 1];
            uint8_t b = _newrequest.strptr[request_starting_pt + 2];
            uint8_t c = _newrequest.strptr[request_starting_pt + 3];
            uint8_t d = _newrequest.strptr[request_starting_pt + 4];

            CLOG_LN(a);
            CLOG_LN(b);
            CLOG_LN(c);
            CLOG_LN(d);
        }

        if (_newrequest.length - (request_length + start_index) >= 14)
        {
            processRequest(_newrequest, request_length + start_index);
        }
    }
}

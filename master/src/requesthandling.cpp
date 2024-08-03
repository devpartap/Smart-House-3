#include "definations.hpp"
#include "esp8266.hpp"
#include "cryptography.hpp"
#include "workercontrol.hpp"

uint8_t websockets_connections[] = {0, 0, 0, 0};
CStrWithSize &requestBuffer = getPrimaryBuffer();
auto ping_time = millis();

void decodeWebsocet(char *_recv_msg, char *_buffer, const uint8_t _buffer_size);

enum Request : const char
{
    WEBSOCKET_MESSAGE = (char)129,
    WEBSOCKET_CLOSING_HANDSHAKE = (char)136,
    WEBSOCKET_PONG = (char)138,
    WEBSOCKET_HANDSHAKE = 'G',
    FLOOR_STATUS_REQUEST = 'F',
    UPDATE_DEVICE = 'U',
    WORKER_STATUS = 'W',
    WORKER_ALTER_DEVICE = 'D'
};

void processRequest(const uint16_t &_stIndex = 0)
{
    bool recursing = false;

    while (true)
    {

        if ((requestBuffer.length - _stIndex) < REQUEST_VALID_SIZE)
        {
            return;
        }

        int16_t start_index = CStrWithSize::indexOf(requestBuffer, "+IPD", _stIndex);
        if (start_index == -1)
        {
            eraseBuffer();
            CLOG_LN("Erased Buffer!");

            return;
        }

        if (recursing)
        {
            espWaitTillFree(true);
        }

        // Processing Request Packet
        CLOG_LN("PROCESSING REQUEST!");
        CLOG("St:");
        CLOG_LN(start_index);

        unsigned char connection_no = requestBuffer[start_index + 5];
        uint16_t request_starting_pt = CStrWithSize::indexOf(requestBuffer, ":", start_index + 8);
        uint16_t request_length = 0;

        for (uint16_t i = start_index + 7; i < request_starting_pt; i++)
        {
            request_length *= 10;
            request_length += requestBuffer[i] - '0';
        }

        char request_type = requestBuffer[request_starting_pt + 1];

        // Logging Request Parameters
        CLOG("C_No:");
        CLOG(connection_no);
        CLOG(", B_Len:");
        CLOG(requestBuffer.length);
        CLOG(", R_St:");
        CLOG(request_starting_pt);
        CLOG(", R_Len:");
        CLOG(request_length);
        CLOG(", R_Typ:");
        CLOG_LN(request_type);

        // Websocket Processing
        if (request_type == Request::WEBSOCKET_MESSAGE)
        {
            uint16_t websocket_msg_offset = 0;

            while (true)
            {
                uint8_t msg_len = (requestBuffer.strptr[request_starting_pt + 2 + websocket_msg_offset]) & 0b01111111;
                CLOG("LEN: ");
                CLOG(msg_len);
                CLOG(", Ofset: ");
                CLOG_LN(websocket_msg_offset);

                char msg[msg_len];
                decodeWebsocet(requestBuffer.strptr + request_starting_pt + websocket_msg_offset + 1, msg, msg_len);

                if (msg[0] == Request::FLOOR_STATUS_REQUEST)
                {
                    uint8_t floor = msg[1] - '0';
                    char to_send_Data[floor_map[floor][1]];

                    getFloorStatus(floor, to_send_Data);
                    sendDataOnWebSocket(connection_no, to_send_Data, floor_map[floor][1]);
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

                    if (updateWorkerDeviceState(decive_change_data))
                    {
                        CLOG_LN("CNF");
                        espWaitTillFree();
                        sendDataOnWebSocket(connection_no, "OK");
                    }
                    else
                    {
                        CLOG_LN("NCF");
                        sendDataOnWebSocket(connection_no, "NK");
                    }
                }

                if ((request_length - 6 - websocket_msg_offset) > msg_len)
                {
                    CLOG_LN("INSIDE1");
                    if (requestBuffer.strptr[request_starting_pt + msg_len + websocket_msg_offset + 7] == Request::WEBSOCKET_MESSAGE)
                    {
                        CLOG_LN("Another Websocket Message!");
                        websocket_msg_offset += msg_len + 6;
                        espWaitTillFree(true);
                        continue;
                    }
                }

                break;
            }
        }
        else if (request_type == Request::WEBSOCKET_HANDSHAKE)
        {
            uint16_t index = CStrWithSize::indexOf(requestBuffer, "ket-Key", 300);
            CStrWithSize key(requestBuffer.strptr + index + 9, 24);

#ifdef _DEBUG_
            CStrWithSize::print(key);
#endif

            espConnectWebSocket(connection_no, processWebsocketKey(key));
            websockets_connections[connection_no - '0'] = PING_TIMOUT;
        }
        else if (request_type == Request::WEBSOCKET_CLOSING_HANDSHAKE)
        {
            CLOG("Closed : ");
            CLOG_LN(connection_no - '0');

            websockets_connections[connection_no - '0'] = 0;
        }

        else if (request_type == Request::WEBSOCKET_PONG)
        {
            CLOG("Pong : ");
            CLOG_LN(connection_no - '0');

            websockets_connections[connection_no - '0'] += 1;
        }

        // Worker Processing
        else if (request_type == Request::WORKER_STATUS)
        {

#ifdef _DEBUG_
            for (uint8_t j = 0; j < request_length; j++)
            {
                CLOG((uint8_t)requestBuffer[request_starting_pt + 1 + j]);
                CLOG('-');
            }
            CLOG_LN(' ');
#endif

            registerWorker((const uint8_t *)(requestBuffer.strptr + request_starting_pt + 2));

            espSendChar(connection_no, 'k');
        }

        else if (request_type == Request::WORKER_ALTER_DEVICE)
        {
            for (uint8_t i = 0; i < 4; i++)
            {
                if (websockets_connections[i])
                {
                    sendDataOnWebSocket(i + '0', requestBuffer.strptr + request_starting_pt + 1, 6);
                    espWaitTillFree();
                }
            }
            CLOG_LN("sending device");
            changeDeviceState((uint8_t *)requestBuffer.strptr + request_starting_pt + 1);
        }
        else
        {
            CLOG_LN("Invalid Request Type!");
        }

        if (requestBuffer.length - (request_length + request_starting_pt) >= REQUEST_VALID_SIZE)
        {
            CLOG("Recursing! : ");
            CLOG(requestBuffer.length);
            CLOG(" - ");
            CLOG(request_starting_pt);
            CLOG(" + ");
            CLOG_LN(request_length);

            recursing = true;
            processRequest(request_length + start_index);
            continue;
        }

        eraseBuffer();
        break;
    }
}

void pingActiveConnections()
{
    if ((millis() - ping_time) > PING_INTERVAL)
    {
        
#ifdef _DEBUG_
    bool tolog = true;
#endif
        
        for (uint8_t i = 0; i < 4; i++)
        {
            if (websockets_connections[i])
            {

#ifdef _DEBUG_
                if(tolog)
                    CLOG("Ping: ");

                tolog = false;
#endif
                sendWebSocketPing(i + '0');
                websockets_connections[i] -= 1; // it will get true if pong is revieved

                CLOG(i);
                CLOG(", ");
                espWaitTillFree();
            }
        }

#ifdef _DEBUG_
        if(!tolog)
            CLOG_LN(' ');
#endif

        ping_time = millis();
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
    for (uint8_t i = 0; i < _buffer_size; i++)
    {
        CLOG(_buffer[i]);
        CLOG('|');
    }
    CLOG('\n');
#endif
}

#include "definations.h"
#include "esp8266.h"
#include "cryptography.h"


#define WEBSOCKET_HANDSHAKE 'G'
#define WEBSOCKET_MESSAGE (char)129
#define WEBSOCKET_CLOSING_HANDSHAKE (char)136
#define FLOOR_STATUS_REQUEST 'F'
#define UPDATE_DEVICE 'U'

bool WebSocketsConnections[] = {false,false,false,false};

void DecodeWebsocet(char* _recv_msg,char* _buffer, const uint8_t _buffer_size);

void processRequest(cStrWithSize _newrequest_,const uint16_t & stIndex = 0)
{    
    _CslLogln("PROCESSING REQUEST!");

    int16_t startIndex = cStr_indexOf(_newrequest_,"+IPD",stIndex);
    _CslLogln(startIndex);

    if(startIndex != -1)
    {
        unsigned char connection_No = _newrequest_[startIndex+5];
        uint16_t request_startingpt = cStr_indexOf(_newrequest_,":",startIndex+8);
        uint16_t request_length = 0;

        for(uint16_t i = startIndex+7; i < request_startingpt; i++)
        {
            request_length *= 10;
            request_length += _newrequest_[i] - '0';
        }

        char request_type = _newrequest_[request_startingpt+1];

        _CslLogln(connection_No);
        _CslLogln(request_startingpt);
        _CslLogln(request_length);
        _CslLog(request_type);
        _CslLog(" : ");
        _CslLogln((uint8_t)_newrequest_.strptr[request_startingpt+1]);

        if(request_type == WEBSOCKET_MESSAGE)
        {
            
            char msg[request_length - 6];
            DecodeWebsocet(_newrequest_.strptr + request_startingpt + 1,msg,request_length - 6);

            if(msg[0] == FLOOR_STATUS_REQUEST)
            {
                if(msg[1] == '0')
                {
                    sendDataOnWebsocket(connection_No,"01");
                }
                else if(msg[1] == '1')
                {
                    sendDataOnWebsocket(connection_No,"0000011100000001");
                }
            }

            else if(msg[0] == UPDATE_DEVICE)
            {
                uint8_t deviceAdress[4] = {0,0,0,0};

                uint8_t infotype = 0;
                uint8_t i = 2;
                for(; i < request_length - 7; i++)
                {
                    if(msg[i] == '.')
                    {
                        infotype++;
                    }
                    else
                    {
                        deviceAdress[infotype] *= 10;
                        deviceAdress[infotype] += msg[i] - '0';
                    }
                }

                bool deviceNewState = msg[i] - '0';

                sendDataOnWebsocket(connection_No, "OK");
                      
            }
        }

        else if(request_type == WEBSOCKET_HANDSHAKE)
        {
            uint16_t index = cStr_indexOf(_newrequest_,"ket-Key",300);
            cStrWithSize key(_newrequest_.strptr +index + 9,24);

#ifdef debug
            cStr_print(key);
#endif
            espConnectWebsocket(connection_No,processWebsocketKey(key));
            WebSocketsConnections[connection_No - '0'] = true; 
            
        }
        else if(request_type == WEBSOCKET_CLOSING_HANDSHAKE)
        {
            WebSocketsConnections[connection_No - '0'] = false;
            _CslLog("Closed : ");
            _CslLogln(connection_No - '0');
        }

        if(_newrequest_.length - (request_length+startIndex) >= 14)
        {
            processRequest(_newrequest_,request_length+startIndex);
        }

    }          
}

void DecodeWebsocet(char* _recv_msg,char* _buffer, const uint8_t _buffer_size)
{

    for(uint8_t i = 0;i < _buffer_size; i++)
    {
        _buffer[i] = (_recv_msg[i+6] ^ _recv_msg[2 + (i % 4)]);
    }

#ifdef debug
    _CslLog("Discrypted char :- ");
    for(int i = 0;i<_buffer_size;i++)
    {
        _CslLog(_buffer[i]);
        _CslLog('|');
    }
    _CslLog('\n');
#endif
        
}



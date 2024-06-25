#include "esp8266.hpp"

char rx_buffer[512];

CStrWithSize espSendRead(const char *_command)
{
    _ESP8266.println(_command);
    _ESP8266.flush();
    delay(10);

    return espRead();
}

CStrWithSize espWaitRead(const char * _command)
{
    _ESP8266.println(_command);
    _ESP8266.flush();
    delay(10);

    while(!_ESP8266.available()) { 
        delay(50);
    }
    
    return espRead();
}

CStrWithSize espRead()
{
#ifdef _DEBUG_
    CStrWithSize temp(rx_buffer, _ESP8266.readBytes(rx_buffer, 512), true);
    LOG_ESP(temp.strptr, temp.length);
    return temp;

#else
    return CStrWithSize(rx_buffer, _ESP8266.readBytes(rx_buffer, 512), true);
#endif
}

void espSend(const char _conection_no,const char * _msg, uint8_t _msglen)
{
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_conection_no);
    _ESP8266.print(",");
    _ESP8266.println(_msglen + 1);
    delay(10);

    _ESP8266.println(_msg);
}

void espSendChar(const char _conection_no,const char _msg)
{
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_conection_no);
    _ESP8266.print(",");
    _ESP8266.println(1);
    delay(10);

    _ESP8266.print(_msg);
}

void espClose(char _conection_no)
{
    _ESP8266.print("AT+CIPCLOSE=");
    _ESP8266.println(_conection_no);
}

int espAvailable()
{
    return _ESP8266.available();
}

void espConnectAP()
{
    if (CStrWithSize::indexOf(espSendRead("AT+CIFSR"), "0.0.0.0") == 1)
    {
        _ESP8266.print("AT+CWJAP=\"");
        _ESP8266.print(g_wifi_ssid);
        _ESP8266.print("\",\"");
        _ESP8266.print(g_wifi_password);
        _ESP8266.println('"');

        _ESP8266.flush();
    
        while(!_ESP8266.available()) { 
            delay(50);
        }
    }


}

void espConnectWebSocket(const char _conection_no, const CStrWithSize &_respondkey)
{

    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_conection_no);
    _ESP8266.print(",");
    _ESP8266.println(113 + 17 + 2 + _respondkey.length);

    delay(10);

    _ESP8266.println("HTTP/1.1 101 Switching Protocols");
    _ESP8266.println("Upgrade: websocket");
    _ESP8266.println("Connection: Upgrade");
    _ESP8266.print("Sec-WebSocket-Accept: ");
    _ESP8266.println(_respondkey.strptr);

    _ESP8266.println("Access-Control-Allow-Origin: *\r\n");

    delete _respondkey.strptr;

    _ESP8266.flush();
}

void sendDataOnWebSocket(const char _connection_no, char *_data,const uint16_t & _size)
{
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_connection_no);
    _ESP8266.print(",");

    if(_size > 125)
    {
        _ESP8266.println(_size + 3);
    }
    else
    {
        _ESP8266.println(_size + 2);
    }
    delay(10);
    
    _ESP8266.print((char)0b10000001);

    if(_size > 125)
    {
        _ESP8266.print((char)(0b01111110));
        _ESP8266.print(_size);
    }
    else
    {
        _ESP8266.print((char)(_size));
    }
       
    _ESP8266.write(_data,_size);

    _ESP8266.flush();
}

void sendDataOnWebSocket(const char _connection_no, const char *_data)
{
    sendDataOnWebSocket(_connection_no,(char*)_data[0], strlen(_data));
}

void connectWroker(const char* _ip)
{
    _ESP8266.print("AT+CIPSTART=4,\"TCP\",\"192.168.");
    _ESP8266.print(_ip);
    _ESP8266.println("\",8080,30");
    _ESP8266.flush();

    while(true) 
    { 
        if(_ESP8266.available())
        {
            if (CStrWithSize::indexOf(espRead(), "4,C") != -1)
                break;
        }
        
    }
  
}

void sendWorkerCommand(const char* _ip,const uint8_t * _command,const uint8_t _size)
{
    connectWroker(_ip);

    _ESP8266.print("AT+CIPSEND=4,");
    _ESP8266.println(_size);
    delay(10);

#ifdef _DEBUG_
    for(uint8_t i = 0; i < _size; i++)
    {
        CLOG(_command[i]);
    }
    CLOG_LN(' ');
#endif

    _ESP8266.write(_command,_size);
    _ESP8266.flush();

}

void connectToIt()
{
    
}

// ----- WORKS, MAYBE BE USED IN FUTURE ------
// void sendWebsocketPing(const char & _connection_no)
// {
//      _ESP8266.print("AT+CIPSEND=");
//     _ESP8266.print(_connection_no);
//     _ESP8266.print(",");
//     _ESP8266.println(3);
//     delay(10);
//     _ESP8266.print((char)0b10001001);
//     _ESP8266.print((char)0b00000001);
//     _ESP8266.print('P');
//     _ESP8266.flush();
// }

// CStrWithSize espWaitRead()
// {
//     while (!_ESP8266.available())
//     {
//         delay(10);
//     }

//     return espRead();
// }

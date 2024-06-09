#include "esp8266.h"

char rx_buffer[512];

CStrWithSize espSendRead(const char *_command)
{
    _ESP8266.println(_command);
    _ESP8266.flush();
    delay(10);

    return espRead();
}

CStrWithSize espRead()
{
#ifdef debug
    CStrWithSize temp(rx_buffer, _ESP8266.readBytes(rx_buffer, 512), true);
    _LogESP(temp.strptr, temp.length);
    return temp;

#else
    return CStrWithSize(rx_buffer, _ESP8266.readBytes(rx_buffer, 512), true);
#endif
}

int espAvailable()
{
    return _ESP8266.available();
}

void espConnectWebSocket(const char &_conection_no, const CStrWithSize &_respondkey)
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

void sendDataOnWebSocket(const char &_connection_no, const char *_data)
{

    uint8_t datalength = strlen(_data);

    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_connection_no);
    _ESP8266.print(",");
    _ESP8266.println(datalength + 2);
    delay(10);

    _ESP8266.print((char)0b10000001);
    _ESP8266.print((char)((datalength) & 0b01111111));
    _ESP8266.println(_data);

    _ESP8266.flush();
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

#include "esp8266.h"

 char rx_buffer[512];


cStrWithSize espSendRead(const char* command)
{
    _ESP8266.println(command);
    _ESP8266.flush();
    delay(10);

    return espRead();
}

cStrWithSize espRead()
{
   
#ifdef debug

    cStrWithSize temp(rx_buffer,_ESP8266.readBytes(rx_buffer,512),true);
    _LogESP(temp.strptr,temp.length);
    return temp;
#else
    return cStrWithSize(rx_buffer,_ESP8266.readBytes(rx_buffer,512),true);
#endif

}

int espAvailable()
{
    return _ESP8266.available();
}

void espConnectWebsocket(const char & _conectionNO_,cStrWithSize _respondkey_)
{
    
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_conectionNO_);
    _ESP8266.print(",");
    _ESP8266.println(113+17+2+_respondkey_.length);

delay(10);
  
  _ESP8266.println("HTTP/1.1 101 Switching Protocols");
  _ESP8266.println("Upgrade: websocket");
  _ESP8266.println("Connection: Upgrade");
  _ESP8266.print("Sec-WebSocket-Accept: ");
  _ESP8266.println(_respondkey_.strptr);

  _ESP8266.println("Access-Control-Allow-Origin: *\r\n");
  
  delete _respondkey_.strptr;

  _ESP8266.flush();
}

void sendDataOnWebsocket( const char & _ConnectionNO,const char * _Data)
{

    uint8_t datalength = strlen(_Data);
    
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_ConnectionNO);
    _ESP8266.print(",");
    _ESP8266.println(datalength + 2);
    delay(10);
    _ESP8266.print((char)0b10000001);
    _ESP8266.print((char)((datalength) & 0b01111111));
    _ESP8266.println(_Data);
    _ESP8266.flush();
    
}

// ----- WORKS, MAYBE BE USED IN FUTURE ------
// void sendWebsocketPing(const char & _ConnectionNO)
// {
//      _ESP8266.print("AT+CIPSEND=");
//     _ESP8266.print(_ConnectionNO);
//     _ESP8266.print(",");
//     _ESP8266.println(3);
//     delay(10);
//     _ESP8266.print((char)0b10001001);
//     _ESP8266.print((char)0b00000001);
//     _ESP8266.print('P');
//     _ESP8266.flush();
// }


// cStrWithSize espWaitRead()
// {
//     while (!_ESP8266.available())
//     {
//         delay(10);
//     }

//     return espRead();
// }

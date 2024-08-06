#include "esp8266.hpp"

char raw_rx_buffer[512];
CStrWithSize buffer(&raw_rx_buffer[0],0);

void espStart()
{

#if ERROR_LED >= 0
    pinMode(ERROR_LED,OUTPUT);
#endif

    _ESP8266.begin(115200);
    _ESP8266.setTimeout(15);

    
    FAIL_CHECK(CStrWithSize::indexOf(espSendRead("AT"), "OK")  == -1,"ESP NOT CONNECTED!");

}

void espSetup()
{
    CLOG_LN("\n=== SETTING UP ESP ===");

#ifdef _DEBUG_
    espSendRead("ATE1");
#else
    espSendRead("ATE0");
#endif

    delay(10);

    espSendRead("AT+CWMODE_CUR=1");
    espSendRead("AT+CIPMUX=1");
    espSendRead("AT+CIPSERVER=1,0080");

    CLOG_LN("Done!");
}

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

void espWaitTillFree(bool check)
{
    if(check)
    {
        if(_ESP8266.available())
            espRead();
            
        if(CStrWithSize::indexOf(espSendRead("AT"),"OK") != -1)
            return;
    }

    auto timespan = millis();
    while((millis() - timespan) <= SEND_WAIT_TIME)
    {
        if(_ESP8266.available())
        {
            CStrWithSize data = espRead();
            if(CStrWithSize::indexOf(data,"SEND OK") != -1)
            {
                return;
            }
            else if(CStrWithSize::indexOf(data,"ERROR") != -1)
            {
                return;
            }
        }   
    }

    CLOG_LN("ESP TAKING TOO LONG");
}

CStrWithSize espRead()
{
    CStrWithSize temp(buffer.strptr + buffer.length,_ESP8266.readBytes(buffer.strptr + buffer.length, 512 - buffer.length), true);
    buffer.length += temp.length + 1 ;

#ifdef _DEBUG_
    LOG_ESP_BUFF(temp);
#endif

    return temp;
}


void espReadToPrimaryBuffer()
{
    if(_ESP8266.available() > REQUEST_VALID_SIZE)
    {
        buffer.length += _ESP8266.readBytes(buffer.strptr + buffer.length, 512 - buffer.length);
        LOG_ESP_BUFF(buffer);
    }
}

void eraseBuffer()
{
    buffer.length = 0;
}

CStrWithSize& getPrimaryBuffer()
{
    return buffer;
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

void espConnectAP()
{
    CLOG_LN("\n=== Connecting To AP ===");
    uint8_t increment = 0;

    while(CStrWithSize::indexOf(espSendRead("AT+CIFSR"), "192.168") == -1)
    {
        if(increment <= 2)
        {
            increment++;
            delay(1000);
            continue;
        }

        _ESP8266.print("AT+CWJAP=\"");
        _ESP8266.print(g_wifi_ssid);
        _ESP8266.print("\",\"");
        _ESP8266.print(g_wifi_password);
        _ESP8266.println('"');

        _ESP8266.flush();

        while(!_ESP8266.available()) { 
            delay(50);
        }

        if(CStrWithSize::indexOf(espRead(),"OK") != -1)
        {
            CLOG_LN("Connected");
            break;
        } 

        if(increment > 6)
            FAIL_CHECK(true,"Could Not Connect To AP");

        increment++;
        delay(2000);
    }

     CLOG_LN("\nDone!");
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

void sendDataOnWebSocket(const char _connection_no, char *_data,const uint16_t & _size,char _command = ' ')
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

    if(_command != ' ')
    {
        _ESP8266.print(_command);
    }
    
    _ESP8266.write(_data,_size);

    _ESP8266.flush();
}

void sendDataOnWebSocket(const char _connection_no, const char *_data)
{
    sendDataOnWebSocket(_connection_no,(char*)_data, strlen(_data));
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
    espWaitTillFree(true);
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


void sendWebSocketPing(const char & _connection_no)
{
    _ESP8266.print("AT+CIPSEND=");
    _ESP8266.print(_connection_no);
    _ESP8266.print(",");
    _ESP8266.println(3);
    delay(10);
    _ESP8266.print((char)0b10001001);
    _ESP8266.print((char)0b00000001);
    _ESP8266.print('P');
    _ESP8266.flush();
}

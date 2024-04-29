#include "definations.h"

#include "esp8266.h"

#define test_led 12
// 192.168.29.167

void setup()
{
    _Console(begin(115200));
    _ESP8266.begin(115200);
    _ESP8266.setTimeout(15);

#ifdef debug
    pinMode(test_led,OUTPUT);
    digitalWrite(test_led,HIGH);
    delay(2000);
    digitalWrite(test_led,LOW);

    _Console(println("<<< Start >>> "));
#endif

    espSendRead("AT+CIPMUX=1");
    espSendRead("AT+CIPSERVER=1,0080");
    espSendRead("AT+CIFSR");

}

enum requestType
{
    STATUS,
    CHANGESTATE
};

struct RequestData
{
    requestType type;
    char connection_No;
    bool to_process;
    String body;


};

RequestData Request[5];

bool decodeRequest(const String & _newrequest_)
{    
    short connection_No = _newrequest_[0];
            
    if(_newrequest_[_newrequest_.indexOf(':') + 1] == 'G')
    {
        type = STATUS;
        body = "";
        to_process = true;
    }
    else
    {
        to_process = false;
    }
}


void loop()
{
    String data = espRead();
    if(data.length() > 100)
    {
        if(data.substring(2,9) == "CONNECT")
            Request.decode(data);
    }

    if(Request.to_process)
    {
        _Console(println("processing!!"));

        if(Request.type == requestType::STATUS)
        {
            espSendData("1010110100",Request.connection_No);
        }
        

        Request.to_process = false;
    }
    
  delay(100);
}


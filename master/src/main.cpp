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
    bool to_process = false;
    long recivedOnMillis;
    String body;

};

RequestData Request[5];



void decodeRequest(const String & _newrequest_,unsigned int startIndex)
{    
    _Console(println("Decode:Inhere"));
    const short connection_No = _newrequest_[startIndex-2] - '0';
            
    if(_newrequest_[_newrequest_.indexOf(':',startIndex) + 1] == 'G')
    {
        Request[connection_No].connection_No = _newrequest_[startIndex-2];
        Request[connection_No].type = STATUS;
        Request[connection_No].to_process = true;
        if(connection_No == 0)
        {
            Request[connection_No].recivedOnMillis = millis();
        }
        else
        {
            Request[connection_No].recivedOnMillis = Request[connection_No - 1].recivedOnMillis + 1000;
            _Console(println(Request[connection_No - 1].recivedOnMillis));
            _Console(println(millis()));
        }
        Request[connection_No].body = "";
        _Console(println(Request[connection_No].recivedOnMillis));

    }
    else
    {
        Request[connection_No].to_process = false;
    }

    if((_newrequest_.length()-startIndex)>400)
    {
        _Console(println("Decode:more_detected"));
        int nxtIndex = _newrequest_.indexOf("CONNECT",200);
        if(nxtIndex != -1)
        {
            _Console(println("Decode:recalling"));
            decodeRequest(_newrequest_,nxtIndex);
        }
    }

}


void loop()
{
    String data = espRead();
    // String data = "0,CONNECT\n\n+IPD,0,316:GET / HTTP/1.1\nHost: 192.168.29.167\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0\nAccept: */*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nOrigin: http://192.168.29.7:5173\nDNT: 1\nSec-GPC: 1\nConnection: keep-alive\nReferer: http://192.168.29.7:5173/\n\n2,CONNECT\n\n+IPD,2,316:GET / HTTP/1.1\nHost: 192.168.29.167\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0\nAccept: */*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nOrigin: http://192.168.29.7:5173\nDNT: 1\nSec-GPC: 1\nConnection: keep-alive\nReferer: http://192.168.29.7:5173/\n";
   
    if(data.length() > 100)
    {
        int stIndex = data.indexOf("CONNECT");
        if(stIndex != -1)
            decodeRequest(data,stIndex);
    }

    // delay(50000);
    for(short i = 0; i<=4;i++)
    {
        if((Request[i].to_process) && (int(millis() - Request[i].recivedOnMillis) >= 5000))
        {   
            _Console(println("processing!!"));
            _Console(println(Request[i].connection_No));

            if(Request[i].type == requestType::STATUS)
            {
                espSendData("1010110100",Request[i].connection_No);
                _ESP8266.flush();
            }
        

            Request[i].to_process = false;
        }
    }
    
  delay(50);
}


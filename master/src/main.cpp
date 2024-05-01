#include "definations.h"

#include "esp8266.h"
#include "requesthandling.h"

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

    _Console(println("<<< - Start - >>> "));
#endif

    espSendRead("ATE1");
    espSendRead("AT+CIPMUX=1");
    espSendRead("AT+CIPSERVER=1,0080");
    espSendRead("AT+CIFSR");
    

}

void loop()
{
    String data = espRead();
    // String data = "0,CONNECT\n\n+IPD,0,317:GET /a HTTP/1.1\nHost: 192.168.29.167\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0\nAccept: */*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nOrigin: http://192.168.29.7:5173\nDNT: 1\nSec-GPC: 1\nConnection: keep-alive\nReferer: http://192.168.29.7:5173/\n\n2,CONNECT\n\n+IPD,2,317:GET /a HTTP/1.1\nHost: 192.168.29.167\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0\nAccept: */*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nOrigin: http://192.168.29.7:5173\nDNT: 1\nSec-GPC: 1\nConnection: keep-alive\nReferer: http://192.168.29.7:5173/\n1,CONNECT\n\n+IPD,1,317:GET /u HTTP/1.1\nHost: 192.168.29.167\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0\nAccept: */*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nOrigin: http://192.168.29.7:5173\nDNT: 1\nSec-GPC: 1\nConnection: keep-alive\nReferer: http://192.168.29.7:5173/\n";
    if(data.length() > 100)
    {
        int stIndex = data.indexOf("+IPD");

        if(stIndex != -1)
            processRequest(data,stIndex);
    }

    // // delay(50000);
    // for(short i = 0; i<=4;i++)
    // {
    //     if((Request[i].to_process) && (int(millis() - Request[i].recivedOnMillis) >= 5000))
    //     {   
    //         _Console(println("processing!!"));
    //         _Console(println(Request[i].connection_No));

    //         if(Request[i].type == requestType::STATUS)
    //         {
    //             espSendData("1010110100",Request[i].connection_No);
    //             _ESP8266.flush();
    //         }
        

    //         Request[i].to_process = false;
    //     }
    // }
    
  delay(50);
}


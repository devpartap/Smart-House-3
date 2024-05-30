#include "definations.h"

#include "esp8266.h"
#include "requesthandling.h"

#define test_led 13
// 192.168.29.167

void errLED();

void setup()
{
    // -------- BOOTING ----------
    pinMode(test_led,OUTPUT);
    
    _ESP8266.begin(115200);
    _ESP8266.setTimeout(15);

    // error checking
#ifndef debug
    if(cStr_indexOf(espSendRead("AT"),"OK") == -1)
    {
        errLED();
    }
#endif

    // setting up ESP
    digitalWrite(test_led,HIGH);
    
#ifdef debug
    _Csl(begin(115200));
    delay(1000);
    _CslLogln("<<< - Start - >>> ");
    espSendRead("ATE1");
#else
    espSendRead("ATE0");
#endif

    espSendRead("AT+CIPMUX=1");
    espSendRead("AT+CIPSERVER=1,0080");
    espSendRead("AT+CIFSR");

    digitalWrite(test_led,LOW);
    
}


void loop()
{
    if(espAvailable() > 14)
    {
        processRequest(espRead());
    }

  delay(50);
}

void errLED()
{
    while(true)
    {
        digitalWrite(test_led,HIGH);
        delay(400);
        digitalWrite(test_led,LOW);
        delay(400);
        digitalWrite(test_led,HIGH);
        delay(400);
        digitalWrite(test_led,LOW);
        delay(1000);
    }
}

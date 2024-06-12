#include "definations.h"

#include "esp8266.h"
#include "requesthandling.h"

#define _TEST_LED_ 13
// 192.168.29.167

void errLED();

void setup()
{
    // -------- BOOTING ----------
    pinMode(_TEST_LED_, OUTPUT);

    _ESP8266.begin(115200);
    _ESP8266.setTimeout(15);

    // error checking
#ifndef _DEBUG_
    if (CStrWithSize::indexOf(espSendRead("AT"), "OK") == -1)
    {
        errLED();
    }
#endif

    // Setting Up master
    digitalWrite(_TEST_LED_, HIGH);

#ifdef _DEBUG_
    CONSOLE(begin(115200));
    delay(1000);
    CLOG_LN("<<< - Start - >>> ");
    espSendRead("ATE1");

#else
    espSendRead("ATE0");
#endif

    espSendRead("AT+CWMODE_CUR=1");
    espSendRead("AT+CIPMUX=1");
    espSendRead("AT+CIPSERVER=1,0080");
    espSendRead("AT+CIFSR");

    digitalWrite(_TEST_LED_, LOW);
}

bool toconect = true;

void loop()
{
    if (espAvailable() > 14)
    {
        processRequest(espRead());
    }

    // if(toconect)
    // {
    //     espWaitRead("AT+CIPSTART=4,\"TCP\",\"192.168.29.29\",8080,300");
    //     toconect = false;
    // }

}

void errLED()
{
    while (true)
    {
        digitalWrite(_TEST_LED_, HIGH);
        delay(400);
        digitalWrite(_TEST_LED_, LOW);
        delay(400);
        digitalWrite(_TEST_LED_, HIGH);
        delay(400);
        digitalWrite(_TEST_LED_, LOW);
        delay(1000);
    }
}

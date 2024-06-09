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
#ifndef debug
    if (CStrWithSize::indexOf(espSendRead("AT"), "OK") == -1)
    {
        errLED();
    }
#endif

    // Setting Up master
    digitalWrite(_TEST_LED_, HIGH);

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

    digitalWrite(_TEST_LED_, LOW);
}

void loop()
{
    if (espAvailable() > 14)
    {
        processRequest(espRead());
    }

    //   delay(50);
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

#pragma once
#include <Arduino.h>

#define debug

#ifdef debug
    #define _CslLog(x) Serial.print(x)
    #define _CslLogln(x) Serial.println(x)
    #define _Csl(x) Serial.x
    #define _LogESP(x,y) _CslLog("\n----- ESP ----- "); _CslLog(y); _CslLog('\n'); _CslLog(x); _CslLog("\n---------------\n")
#else
    #define _CslLog(x)
    #define _CslLogln(x)
    #define _Csl(x)
    #define _LogESP(x,y) 
#endif

#define _ESP8266 Serial3

struct cStrWithSize
{
    
    char * strptr = nullptr;
    uint16_t length = 0;

    cStrWithSize(char * _str, const int16_t & _strsize,bool toNullTerminate = false)
        :strptr(_str), length(_strsize)
    {  
        if(toNullTerminate)
            strptr[length] = '\0';
    }

    char operator[](const uint16_t & indx){
        return strptr[indx];
    }

    cStrWithSize()
    { }
};


int16_t cStr_indexOf(const cStrWithSize& _bsting,const char * _tofindindex,const uint16_t & _stIndex = 0);

#ifdef debug
    void cStr_print(cStrWithSize & _bstring);
#endif
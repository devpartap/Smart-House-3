#pragma once
#include <Arduino.h>

#define _DEBUG_
// -- _debuggingging Stuff ---
#ifdef _DEBUG_

#define CLOG(x) Serial.print(x)
#define CLOG_LN(x) Serial.println(x)
#define CONSOLE(x) Serial.x
#define LOG_ESP(x, y)              \
    CLOG("\n----- ESP ----- "); \
    CLOG(y);                    \
    CLOG('\n');                 \
    CLOG(x);                    \
    CLOG("\n---------------\n")

#else
#define CLOG(x)
#define CLOG_LN(x)
#define CONSOLE(x)
#define LOG_ESP(x, y)
#endif

#define _ESP8266 Serial3

struct CStrWithSize
{
    char *strptr = nullptr;
    uint16_t length = 0;

    CStrWithSize(char *_str, const int16_t &_strsize, bool toNullTerminate = false)
        : strptr(_str), length(_strsize)
    {
        if (toNullTerminate)
            strptr[length] = '\0';
    }

    CStrWithSize() = default;

    char operator[](const uint16_t &indx) const
    {
        return strptr[indx];
    }

    static int16_t indexOf(const CStrWithSize &_bsting, const char *_tofindindex, const uint16_t &_stIndex = 0);

#ifdef _DEBUG_
    static void print(CStrWithSize &_bstring);
#endif
};

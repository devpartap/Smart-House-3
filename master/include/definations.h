#pragma once
#include <Arduino.h>

#define debug

#ifdef debug
    #define _Console(x) Serial.x
    #define _LogESP(x) Serial.println("\n----- ESP -----" + String(x.length()) +"\n" + x + "---------------\n");
#elif
    #define _Console(x) 
    #define _LogESP(x) 
#endif

#define _ESP8266 Serial3
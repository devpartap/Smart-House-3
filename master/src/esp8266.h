#pragma once

#include "definations.h"

cStrWithSize espRead();
cStrWithSize espSendRead(const char* command);
// cStrWithSize espWaitRead();

int espAvailable();

void espConnectWebsocket(const char & _conectionNO_,cStrWithSize _respondkey_);
void sendDataOnWebsocket( const char & _ConnectionNO,const char * _Data);

// void sendWebsocketPing(const char & _ConnectionNO);
#pragma once

#include "definations.hpp"

CStrWithSize espRead();
CStrWithSize espSendRead(const char *_command);
CStrWithSize espWaitRead(const char * _command);
// CStrWithSize espWaitRead();

int espAvailable();

void espConnectWebSocket(const char &_conection_no, const CStrWithSize &_respondkey);
void sendDataOnWebSocket(const char &_connection_no, const char *_data);

// void sendWebsocketPing(const char & _ConnectionNO);
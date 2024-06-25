#pragma once

#include "definations.hpp"

CStrWithSize espRead();
CStrWithSize espSendRead(const char *_command);
CStrWithSize espWaitRead(const char * _command);

void espSend(char _conection_no,const char * _msg, uint8_t _msglen);
void espSendChar(char _conection_no,const char _msg);

int espAvailable();
void espClose(char _conection_no);
void espConnectAP();

void espConnectWebSocket(const char _conection_no, const CStrWithSize &_respondkey);

void sendDataOnWebSocket(const char _connection_no, const char *_data);
void sendDataOnWebSocket(const char _connection_no, char *_data,const uint16_t & _size);

void sendWorkerCommand(const char* ip,const char* command);

// void sendWebsocketPing(const char & _ConnectionNO);
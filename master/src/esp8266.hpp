#pragma once

#include "definations.hpp"

void espStart();
void espSetup();
void espReadToPrimaryBuffer();

CStrWithSize espRead();
CStrWithSize& getPrimaryBuffer();
CStrWithSize espSendRead(const char *_command);
CStrWithSize espWaitRead(const char * _command);

void espSend(char _conection_no,const char * _msg, uint8_t _msglen);
void espSendChar(char _conection_no,const char _msg);

void eraseBuffer();
void espWaitTillFree(bool check = false);

void espClose(char _conection_no);
void espConnectAP();

void espConnectWebSocket(const char _conection_no, const CStrWithSize &_respondkey);
void sendDataOnWebSocket(const char _connection_no, const char *_data);
void sendDataOnWebSocket(const char _connection_no, char *_data,const uint16_t & _size);
void sendWebSocketPing(const char & _ConnectionNO);

void sendWorkerCommand(const char* _ip,const uint8_t * _command,const uint8_t _size);


#pragma once

#include "definations.h"

String espRead();
String espSendRead(const String &command);
String espWaitRead();

void espSendData(const String & _Data_,const char & _conectionNO_);
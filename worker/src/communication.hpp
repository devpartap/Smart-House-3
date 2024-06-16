#pragma once

void sendBoardStatus(uint8_t * _switchs_status);
void connectToWiFi();
void connectToMaster();

void listenForMaster();

void startServer();
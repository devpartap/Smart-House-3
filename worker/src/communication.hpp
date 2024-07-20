#pragma once

void connectToWiFi();
void startServer();
void reportToMaster();
void listenForMaster();

void sendBoardStatus(uint8_t * _switchs_status);
void sendDeviceAlterReport(const uint8_t device_no, const bool new_state);



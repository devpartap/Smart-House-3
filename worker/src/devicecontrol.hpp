#pragma once

void setupEEPROM();
void setupSwitches();

void listenSwitchChange();

void compressDevicesState(char * arry);
void changeDeviceState(uint8_t deviceNo,bool state);


#pragma once

void registerWorker(const uint8_t *_worker_data);
void getFloorStatus(uint8_t floor,char* to_send);

bool chnageDeviceState(const uint8_t * _worker_data);
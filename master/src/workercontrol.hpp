#pragma once

void registerWorker(const uint8_t *_worker_data);
void getFloorStatus(uint8_t floor,char* to_send);

bool updateWorkerDeviceState(const uint8_t * _worker_data);
void changeDeviceState(const uint8_t * _worker_data);
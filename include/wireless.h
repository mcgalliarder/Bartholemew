#ifndef _WIRELESS_H_
#define _WIRELESS_H_

#define LIDAR_INFO 0x25
#define CHASSIS_CMD 0x35

int initTransmitter();

void controllerSend(int * payload);

void wirelessReceive(void * n);

#endif

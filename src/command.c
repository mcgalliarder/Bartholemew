#include "main.h"
#include "wireless.h"
#include "command.h"

void handleLidarInfo(int * payload);
void handleChassisCmd(int * payload);

void decodeCommand(int * payload) {
  int command = payload[0];
  switch(command) {
    case LIDAR_INFO: handleLidarInfo(payload); break;
    case CHASSIS_CMD: handleChassisCmd(payload); break;
  }
}

void handleLidarInfo(int * payload) {

}

void handleChassisCmd(int * payload) {
  printf("Setting Chassis...\n");
  chassisSet(payload[1], payload[1], payload[2], payload[2]);
  //TODO:
}

#include "main.h"
#include "i2cTask.h"

#define INPORT 11
#define OUTPORT 12

void initializeIO() {
  watchdogInit();
  // Initialize ultrasonic sensor
  sonar = ultrasonicInit(OUTPORT, INPORT);
  if (sonar == NULL)
    printf("Sensor initialization Failed\n");

  // Initialize shaft encoders
  int portTop = 2;
  int portBottom = 1;
  baseEncoder = encoderInit(portTop, portBottom, false);
}

void initialize() {
  TaskHandle kill = taskCreate(thread, TASK_MINIMAL_STACK_SIZE,
    NULL, TASK_PRIORITY_DEFAULT);

  i2cTaskStart();
}

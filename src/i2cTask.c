#include "main.h"
#include "i2cTask.h"

#define NUM_IMES 2 //using two IMEs on the robot
#define IME_LEFT 0
#define IME_RIGHT 1

#define CYCLE_TIME 2 //loop delay in milliseconds

volatile int leftIME, rightIME;
TaskHandle i2ctask;
static void i2cHandler(void* ignore) {
  int num_IMEs_initialized = imeInitializeAll();
  if (num_IMEs_initialized != NUM_IMES) {
    printf("ERROR: INCORRECT NUMBER OF IMEs INITIALIZED\n");
  }

  uint32_t now = millis();
  int left;
  int right;
  while(true) {
    imeGet(IME_LEFT, &left);
    imeGet(IME_RIGHT, &right);

    //printf("Left: %d\n", left);
    //printf("Right: %d\n", right);

    leftIME = left;
    rightIME = right;

    taskDelayUntil(&now, CYCLE_TIME);
  }
}

void killTask(){
  taskDelete(i2ctask);
}

void i2cTaskStart() {
  i2ctask = taskCreate(i2cHandler, TASK_DEFAULT_STACK_SIZE, NULL, (TASK_PRIORITY_DEFAULT + 2));
}

int getLeftIME() {
  return leftIME;
}

int getRightIME() {
  return rightIME;
}

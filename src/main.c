#include "arm.h"
#include "chassis.h"
#include "lineTrack.h"
#include "sonar.h"
#include "avoidance.h"
#include "i2cTask.h"
#include "main.h"
#include "armControl.h"

int auton,bP = 1;
int count = 0;
int distance = 0;
int go = false;

volatile int butt = true;
long lastTime;

void operatorControl() {
  int leftPower;
  int leftTurn;

  int rightPower;
  int rightTurn;

  int threshold = 25;

  while (1) {
    butt = true;
    /*______________________JOYSTICKS_________________________*/

    //LEFT: Left Drive Control
    if (abs(joystickGetAnalog(CONTROLLER, 3)) > threshold)
      leftPower = joystickGetAnalog(CONTROLLER, 3);
    else leftPower = 0;

    if (abs(joystickGetAnalog(CONTROLLER, 4)) > threshold)
      leftTurn = joystickGetAnalog(CONTROLLER, 4);
    else leftTurn = 0;

    //RIGHT: Right Drive Control
    if (abs(joystickGetAnalog(CONTROLLER, 2)) > threshold)
      rightPower = joystickGetAnalog(CONTROLLER, 2);
    else rightPower = 0;

    if (abs(joystickGetAnalog(CONTROLLER, 1)))
      rightTurn = joystickGetAnalog(CONTROLLER, 1);
    else rightTurn = 0;

    chassisSet(leftPower+leftTurn, leftPower-leftTurn,
               rightPower+rightTurn, rightPower-rightTurn);

    /*________________________BUMPERS_________________________*/
    //LEFT-UP: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUMP, JOY_UP)) {}
    //LEFT-DOWN: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUMP, JOY_DOWN)) {}
    //RIGHT-UP: Unbound
    if (joystickGetDigital(CONTROLLER, RIGHT_BUMP, JOY_UP)) {}
    //RIGHT-DOWN: Unbound
    if (joystickGetDigital(CONTROLLER, RIGHT_BUMP, JOY_DOWN)) {}


    /*_____________________RIGHT_BUTTONS______________________*/
    //UP: Enter Arm Control
    if (joystickGetDigital(CONTROLLER, RIGHT_BUTT_SET, JOY_UP)) {
      go = true;
      armControl(go);
    }
    //DOWN: Autonomous (Hold)
    if (joystickGetDigital(CONTROLLER, RIGHT_BUTT_SET, JOY_DOWN)) {
      autonomous();
    }
    //LEFT: Unbound
    if (joystickGetDigital(CONTROLLER, RIGHT_BUTT_SET, JOY_LEFT)) {}
    //RIGHT: Program termination
    if (joystickGetDigital(CONTROLLER, RIGHT_BUTT_SET, JOY_RIGHT)) {
      killTask();
      break;
    }

    /*______________________LEFT_BUTTONS______________________*/
    //UP: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUTT_SET, JOY_UP)) {}
    //DOWN: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUTT_SET, JOY_DOWN)) {}
    //LEFT: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUTT_SET, JOY_LEFT)) {}
    //RIGHT: Unbound
    if (joystickGetDigital(CONTROLLER, LEFT_BUTT_SET, JOY_RIGHT)) {}

    delay(20);
  }
}

/******************************************************************
* Transfers control over to the robot
******************************************************************/
void autonomous() {
  delay(500);
  lineTrack();
}

void thread(void * p) {
  int buttonPin = 10;
  while(true) {
    if (!digitalRead(buttonPin)) {
      butt = false;
    }
    delay(500);
  }
}

int checkButton() {
  return butt;
}

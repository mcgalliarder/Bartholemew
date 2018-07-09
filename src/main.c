#include "arm.h"
#include "chassis.h"
#include "lineTrack.h"
#include "sonar.h"
#include "avoidance.h"
#include "i2cTask.h"
#include "main.h"
#include "armControl.h"

int controller = 1;
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
    if (abs(joystickGetAnalog(controller, 3)) > threshold)
      leftPower = joystickGetAnalog(controller, 3);
    else leftPower = 0;

    if (abs(joystickGetAnalog(controller, 4)) > threshold)
      leftTurn = joystickGetAnalog(controller, 4);
    else leftTurn = 0;

    //RIGHT: Right Drive Control
    if (abs(joystickGetAnalog(controller, 2)) > threshold)
      rightPower = joystickGetAnalog(controller, 2);
    else rightPower = 0;

    if (abs(joystickGetAnalog(controller, 1)))
      rightTurn = joystickGetAnalog(controller, 1);
    else rightTurn = 0;

    chassisSet(leftPower+leftTurn, leftPower-leftTurn,
               rightPower+rightTurn, rightPower-rightTurn);

    /*________________________BUMPERS_________________________*/

    //LEFT: Shoulder Control
    if (joystickGetDigital(controller, LEFT_BUMP, JOY_UP)) {
      printf("Right: %d\n", getRightIME());
      printf("Left: %d\n", getLeftIME());
    }

    //RIGHT: Elbow Control
    if (joystickGetDigital(controller, RIGHT_BUMP, JOY_UP)) {
      autonomous();
      //rearTurnOrient(getDistance());
    }

    if (joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_LEFT)) {
      //calcOrient();
      traversal(40, true);
    }

    if (joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_UP)) {
      imeReset(IME_FRONT_RIGHT);
      imeReset(IME_REAR_LEFT);
    }

    //DOWN: Autonomous (Hold)
    if (joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_DOWN))
    {
      go = true;
      //autonomous();
      armControl(go);
    }

    //RIGHT: Program termination
    if (joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_RIGHT)) {
      killTask();
      break;
    }

    //UP: Program termination
    if (joystickGetDigital(controller, LEFT_BUTT_SET, JOY_UP)) {
      printf("Sonar Value: %d\n", getAveragedDistance());
    }

    //Axis turn to the right
    if (joystickGetDigital(controller, LEFT_BUTT_SET, JOY_RIGHT)) {
      rearTurn(40);
    }

    //Axis turn to the left
    if (joystickGetDigital(controller, LEFT_BUTT_SET, JOY_LEFT)) {
        aTurn(0,40,40);
    }

    delay(20);
  }
}

/******************************************************************
* Transfers control over to the robot
******************************************************************/
void autonomous() {
  int scanDistance = 12;

  //Initialize Scan
  //initScanPosition();

  //Move Forward until object detection
  chassisSet(50, 50, 50, 50);
  while(getValidDistance() > scanDistance);
  chassisSet(0,0,0,0);
  while (getValidDistance() < 30 &&
          !joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_RIGHT))
    avoid();
  delay(500);
  /*
  chassisSet(0, 0, 0, 0);
  while (!joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_RIGHT)) {
    avoidPD(calcError(getAveragedDistance()));
  }
  chassisSet(0, 0, 0, 0);
  basePosition();
  delay(500);
  */
  /*
  traversal(40, false);
  long currentTime;
  while (!joystickGetDigital(controller, RIGHT_BUTT_SET, JOY_RIGHT)) {
    currentTime = millis();
    //int elapsed = currentTime - lastTime;
    lastTime = currentTime;
    printf("Elapsed: %ld\n", currentTime);
    printf("Distance: %d\n\n", getAveragedDistance());
  }
  */

  delay(500);

}

/******************************************************************
* Does something
******************************************************************/
void thread(void * p) {
  int buttonPin = 10;
  while(true) {
    if (!digitalRead(buttonPin)) {
      //encoderReset(baseEncoder);
      butt = false;
    }
    delay(500);
  }
}

int checkButton() {
  return butt;
}

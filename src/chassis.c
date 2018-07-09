#include "main.h"
#include "chassis.h"
#include "i2cTask.h"

#define LEFTFRONT 10
#define LEFTREAR 1
#define RIGHTFRONT 6
#define RIGHTREAR 5
#define HIGH_BOUND 127
#define LOW_BOUND -127

int currentSpeed;
int lastSpeed;
int upd = 40;
int leftEnc = 0;
int rightEnc = 0;

/******************************************************************
* Sets the speed of each individual wheel
******************************************************************/
void chassisSet(int leftFrontSpeed, int leftRearSpeed,
                int rightFrontSpeed, int rightRearSpeed)
{
  leftFrontSet(leftFrontSpeed);
  leftRearSet(leftRearSpeed);
  rightFrontSet(rightFrontSpeed);
  rightRearSet(rightRearSpeed);
}

/******************************************************************
* Performs a rear turn
******************************************************************/
void rearTurn(int speed) {
    chassisSet(0, speed, 0, -speed);
}

void rearTurnDegree(int speed, int degree) {
  if (degree == 0)
    return;
  if (!resetEncoders())
      print("Encoder Reset Failure\n");
  delay(200);
  int angle = -degree * 77;
  angle = abs(angle);
  if (degree < 0)
    rearTurn(speed);
  else
    rearTurn(-speed);

  unsigned long checkTime = millis() + 3000;
  while(abs(getRightIME()) < angle && checkEncoders(checkTime)) delay(100);
  chassisSet(0,0,0,0);
}

/******************************************************************
* Performs an axis turn
******************************************************************/
void aTurn(int right, int leftSpeed, int rightSpeed) {
  if (right)
    chassisSet(leftSpeed, leftSpeed, -rightSpeed, -rightSpeed);
  else
    chassisSet(-leftSpeed, -leftSpeed, rightSpeed, rightSpeed);
}

/******************************************************************
* Perform an axis turn to a specified angle
******************************************************************/
void aTurnDegree(int leftSpeed, int rightSpeed, int degree) {
  if (!resetEncoders())
    print("Encoder Reset Failure\n");
  int angle = degree * upd;
  angle = abs(angle);
  printf("HELLO: %d\n", getLeftIME());
  if (degree > 0)
    chassisSet(leftSpeed, leftSpeed, -rightSpeed, -rightSpeed);
  else
    chassisSet(-leftSpeed, -leftSpeed, rightSpeed, rightSpeed);

  unsigned long checkTime = millis() + 3000;
  while (abs(getLeftIME()) < angle && abs(getRightIME()) < angle &&
          checkEncoders(checkTime)) delay(200);
}

/******************************************************************
* Translational movement using two opposing wheels
******************************************************************/
void tMove(int speed, int direction) {
  switch(direction) {
    case 0: chassisSet(speed, 0, 0, speed); //Forward Right
            break;
    case 1: chassisSet(-speed, 0, 0, -speed); //Back Left
            break;
    case 2: chassisSet(0, speed, speed, 0); //Forward Left
            break;
    case 3: chassisSet(0, -speed, -speed, 0); //Back Right
            break;
    default: printf("Not a direction.");
  }
}

/******************************************************************
* Traversing movement using all wheels
******************************************************************/
void traversal(int speed, int right) {
  if (right)
    chassisSet(speed, -speed, -speed, speed);
  else
    chassisSet(-speed, speed, speed, -speed);
}

void traversalCm(int speed, int centimeters) {
  if (!resetEncoders())
    print("Encoder Reset Failure\n");
  int encVal = centimeters * 71;
  encVal = abs(encVal);

  if (centimeters > 0)
    encTraversal(speed, false, encVal);
  else
    encTraversal(speed, true, encVal);

}

/******************************************************************
* Traversing movement up to specified encoder value
******************************************************************/
void encTraversal(int speed, int right, int encVal) {
  traversal(speed, right);
  unsigned long checkTime = millis() + 3000;
  while (abs(getLeftIME()) < encVal && abs(getRightIME()) < encVal &&
         checkEncoders(checkTime)) delay(200);

  chassisSet(0, 0, 0, 0);
}

int checkEncoders(unsigned int checkTime) {
  if (millis() >= checkTime) {
    if (getLeftIME() == leftEnc && getRightIME() == rightEnc) {
      printf("Encoder Read Failure - L: %d, R: %d\n", leftEnc, rightEnc);
      leftEnc = rightEnc = 0;
      return false;
    }
    else
      return true;
  }
  else {
    leftEnc = getLeftIME();
    rightEnc = getRightIME();
    return true;
  }
}

int resetEncoders() {
  imeReset(IME_FRONT_RIGHT);
  imeReset(IME_REAR_LEFT);
  delay(300);
  return (abs(getLeftIME()) < 200 && abs(getRightIME()) < 200);
}

void rightMotorSet(int speed)
{
  rightFrontSet(speed);
  rightRearSet(speed);
}

void leftMotorSet(int speed)
{
  leftFrontSet(speed);
  leftRearSet(speed);
}

void leftFrontSet(int speed)
{
  speed = boundCheck(speed);
  motorSet(LEFTFRONT, speed);
  lastSpeed = currentSpeed;
  currentSpeed = speed;
}

void leftRearSet(int speed)
{
  speed = boundCheck(speed);
  motorSet(LEFTREAR, speed);
  lastSpeed = currentSpeed;
  currentSpeed = speed;
}

void rightFrontSet(int speed)
{
  speed = boundCheck(speed);
  motorSet(RIGHTFRONT, speed);
  lastSpeed = currentSpeed;
  currentSpeed = speed;
}

void rightRearSet(int speed)
{
  speed = boundCheck(speed);
  motorSet(RIGHTREAR, speed);
  lastSpeed = currentSpeed;
  currentSpeed = speed;
}

/******************************************************************
* Checks the bounds of the passed speed parameter -
* sets to max/min value when value is outside bounds
******************************************************************/
int boundCheck(int speed) {
  if (speed > HIGH_BOUND) speed = HIGH_BOUND;
  if (speed < LOW_BOUND) speed = LOW_BOUND;
  return speed;
}

int getLeftFront()
{
  return motorGet(LEFTFRONT);
}

int getLeftRear()
{
  return motorGet(LEFTREAR);
}

int getRightFront()
{
  return motorGet(RIGHTFRONT);
}

int getRightRear()
{
  return motorGet(RIGHTREAR);
}

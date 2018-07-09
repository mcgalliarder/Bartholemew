#include "main.h"
#include "arm.h"

//Motors
#define BASE_MOTOR1 2
#define BASE_MOTOR2 3
#define MID_MOTOR 9
#define TOP_MOTOR 8
//Sensors
#define MID_POT 1
#define TOP_POT 2

//Presets
/******************************************************************
* Sets the arm to a scan position
******************************************************************/
void initScanPosition() {
  int base = 90;
  int mid = 1100;
  int top = manAdjustSonar(base, mid) - 200;
  setAllAxis(base, mid, top); //top:3128
}

/******************************************************************
* Returns the arm to the base position
******************************************************************/
void basePosition() {
  int base = 0;
  int mid = 700;
  int top = manAdjustSonar(base, mid) + 300;
  //Clear
  setAllAxis(base, mid+300, 3000);
  setAllAxis(base, mid, top);
}

//Motor Mutators
/******************************************************************
* Set the axis of the arm to specified values
******************************************************************/
void setAllAxis(int basePos, int midPos, int topPos) {
  int isBaseSet = false;
  int isMidSet = false;
  int isTopSet = false;
  while((!isBaseSet || !isMidSet || !isTopSet) && checkButton()) {
    if (!isBaseSet)
      isBaseSet = setBaseAxis(basePos, 40);
    else {
      if (getBaseEncoder() > 50)
        setBase(-15);
      else
        setBase(15);
    }
    if (!isMidSet)
      isMidSet = setMidAxis(midPos, 40);
    else
      setMid(10);
    if (!isTopSet)
      isTopSet = setTopAxis(topPos);
    else
      setTop(7);
  }
  stopArm();
}

void setMid(int speed) { motorSet(MID_MOTOR, -speed); }
void setTop(int speed) { motorSet(TOP_MOTOR, -speed); }
void setBase(int speed) {
  motorSet(BASE_MOTOR1, speed);
  motorSet(BASE_MOTOR2, -speed);
}

/******************************************************************
* Stops movement of arm
******************************************************************/
void stopArm() {
  if (getBaseEncoder() > 50)
    setBase(-10);
  else
    setBase(10);

  setMid(10);
  setTop(7);
}


//Sensor-Motor Mutators
void resetBase() { encoderReset(baseEncoder); }

int setBaseAxis(int value, int speed) {
  if (getBaseEncoder() < value) {
    if (getBaseEncoder() > 50)
      setBase(30);
    else
      setBase(35);
  } else if (getBaseEncoder() > value) {
    if (getBaseEncoder() > 50)
      setBase(-35);
    else
      setBase(-30);
  }


  return ((getBaseEncoder() >= value-5) && (getBaseEncoder() <= value+5));
}

/******************************************************************
*
******************************************************************/
int setMidAxis(int value, int speed) {
  if (getMidPot() < value)
    setMid(speed);
  else if (getMidPot() > value)
    setMid(-speed);

  return ((getMidPot() >= value-10) && (getMidPot() <= value+10));
}

/******************************************************************
*
******************************************************************/
int setTopAxis(int value) {
  if (getTopPot() < value)
    setTop(18);
   else
    setTop(-15);

  return ((getTopPot() >= value-5) && (getTopPot() <= value+5));
}

//Sensor Accessors
int getMidPot() { return analogRead(MID_POT); }
int getTopPot() { return analogRead(TOP_POT); }
int getBaseEncoder() { return encoderGet(baseEncoder); }

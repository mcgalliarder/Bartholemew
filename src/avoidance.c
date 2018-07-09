#include "arm.h"
#include "avoidance.h"
#include "chassis.h"
#include "i2cTask.h"
#include "main.h"
#include "sonar.h"
#include <math.h>

#define AVOID_DISTANCE 8;
#define PI 3.14159
int tDistance = 30;
int baseS = 50;
int lastError;
float lastTime;

/******************************************************************
*  Returns the distance from the current location to the desired -
*  location
******************************************************************/
int calcError(int currentDist) {
  // if (currentDist > 1000) rearTurnOrient(currentDist);
  // int error = currentDist - AVOID_DISTANCE;
  return currentDist - AVOID_DISTANCE;
}

/******************************************************************
*  Keeps the object at a specified distance away from an object -
*  while also traversing along the object
******************************************************************/
void avoidPD(int error) {
  double KP = .5;
  double KD = 1;
  float derivative = 0;
  int traverse = 0;
  int orient = 0;
  // int baseSpeed = 0;

  // Derivative calculation
  if (error != lastError) {
    int currentTime = millis();
    float seconds = ((currentTime - lastTime) / 1000);
    lastTime = currentTime;

    if (seconds != 0)
      derivative = error / seconds;

    if (abs(lastError) > abs(error))
      derivative = -derivative;
    lastError = error;
  }
  traverse = (KP * error);
  orient = (KD * derivative);

  int leftFront = -baseS + traverse;
  int rightFront = baseS + traverse;
  int leftRear = baseS + traverse + orient;
  int rightRear = -baseS + traverse - orient;

  chassisSet(leftFront, leftRear, rightFront, rightRear);

  printf("Error: %d\n", error);
  printf("Derivative: %d\n", orient);
  printf("Proportion: %d\n", traverse);
  printf("Left Front: %d\n", leftFront);
  printf("Left Rear: %d\n", leftRear);
  printf("Right Front: %d\n", rightFront);
  printf("Right Rear: %d\n\n", rightRear);
}

/******************************************************************
*  Orients the robot to face the object. Turns only the front
*  wheels TODO: Need to change name to frontTurn and find/replace
******************************************************************/
void rearTurnOrient(int currentDist) {
  int currentDistance = currentDist;
  int lowestDist = currentDistance > 1000 ? currentDistance : 1000;
  chassisSet(0, baseS, 0, -baseS);
  if (getAveragedDistance() < lowestDist) {
    while (currentDistance <= lowestDist) {
      chassisSet(baseS, 0, -baseS, 0);
      currentDistance = getAveragedDistance();
      if (currentDistance < lowestDist)
        lowestDist = currentDistance;
    }
  } else {
    while (currentDistance <= lowestDist) {
      chassisSet(-baseS, 0, baseS, 0);
      currentDistance = getAveragedDistance();
      if (currentDistance < lowestDist)
        lowestDist = currentDistance;
    }
  }
}

/******************************************************************
*  Method to procedurally maneuver around a square object
******************************************************************/
void proceduralAvoidance() {
  int avoidDistance = 8;
  // Initialize Scan
  initScanPosition();

  // Move Forward until object detection
  chassisSet(50, 50, 50, 50);
  while (getAveragedDistance() > avoidDistance)
    ;
  chassisSet(0, 0, 0, 0);

  // Traverse parallel to the object
  traverseUntilLoss();

  // Turn 90 degrees
  aTurnDegree(50, 50, 90);

  // tMove around the object till detection
  tMove(80, 1);
  while (getAveragedDistance() > avoidDistance + 15)
    ;
  chassisSet(0, 0, 0, 0);

  traverseUntilLoss();
  printf("Dist: %d\n", getAveragedDistance());

  aTurnDegree(50, 50, -90);

  tMove(80, 0);
  delay(4000);
}

void traverseUntilLoss() {
  do {
    traversal(50, false);
  } while (getAveragedDistance() <= tDistance);
}

/******************************************************************
* Turn the robot until it faces the nearest point
******************************************************************/
void axisTurnUntilFound() {
  // int previousDistance = getAveragedDistance();
  int currentDistance = 0;
  int lowestDist = 1000000000;
  do {
    aTurn(true, 50, 50);
    currentDistance = getAveragedDistance();
    if (currentDistance < lowestDist)
      lowestDist = currentDistance;
    // previousDistance = currentDistance;
    printf("Current Dist: %d\n", currentDistance);
    printf("Low Dist: %d\n\n", lowestDist);
  } while (currentDistance <= lowestDist);
}

/******************************************************************
* Maneuvers around corners
******************************************************************/
void corner() {
  while (getAveragedDistance() > 20)
    chassisSet(0, 40, 0, -40);
}

void avoid() {
  int optimumDistance = 12;
  resetEncoders();
  int initTravDist = 7;

  if (getValidDistance() < 25) {
    // Get initial Distance
    int initDist = getValidDistance();
    printf("initDist: %d\n", initDist);
    traversalCm(40, initTravDist);

    // Get next distance & calculate error
    int endDist = getValidDistance();
    printf("endDist: %d\n", endDist);
    int error = endDist - initDist;
    printf("error: %d\n", error);

    // Orient
    if (endDist != initDist)
      orient(error, initTravDist);

    // Correct
    correctDistance(optimumDistance);
    printf("correctedDist: %d\n\n", getValidDistance());
  } else
    cornerTrack();


}

void printData() {

}

void correctDistance(int optimumDistance) {
  int current = getValidDistance();
  if (current > optimumDistance+1)
    chassisSet(40, 40, 40, 40);
  else if (current < optimumDistance-1)
    chassisSet(-40, -40, -40, -40);
  while (getValidDistance() != optimumDistance)
    ;
  chassisSet(0, 0, 0, 0);
}

/******************************************************************
* Trace along the side of an object while maintaining certain -
* distance
******************************************************************/
void orient(double error, double travDist) {
  double x = error / travDist;
  double rads = atan(x);
  int angle = (int)((180 / PI) * rads);

  printf("Orient Angle: %d\n", angle);
  // turn!
  rearTurnDegree(40, -angle);
}

void cornerTrack() {
  rearTurnDegree(40, 90);
  printf("EndCorner Distance: %d\n", getValidDistance());
  if (getValidDistance() > 50) {
    traversal(40, false);
    while(getValidDistance() > 30);
  }
}

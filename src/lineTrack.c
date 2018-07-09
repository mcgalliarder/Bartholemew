#include "chassis.h"
#include "lineTrack.h"
#include "main.h"

#define RIGHT_FRONT 6
#define LEFT_CENTER 7
#define LEFT_FRONT 8
#define LEFT_REAR 5
#define RIGHT_CENTER 3
#define RIGHT_REAR 4

#define FRONT_LEFT 0x01
#define FRONT_RIGHT 0x02
#define CENTER_LEFT 0x04
#define CENTER_RIGHT 0x08
#define REAR_LEFT 0x10
#define REAR_RIGHT 0x20

#define LEFT 0
#define RIGHT 1

static float KP = 0.15; //.15 //0.3 3/20
static float KD = 0.6; //0.6 3/20
//static float KI = 0.0001;

int whiteTape = true;

int baseSpeed = 50;
int threshold = 800;
int lastError;
int lastRearError;
int lastTime;
int printCount = 0;
int error;
//int
// Flag to invert right angle turing
int rightInversion = 0;

void errorCorrection() {
  error = 0;
  int degree[] = {190, 550, 700}; //3 5 6

  int sensors = sensorDetection();

  /******* PD Control *******/
  switch (sensors) {
  // Single sensor correction
  case FRONT_RIGHT:
    error = degree[0];
    break;
  case FRONT_LEFT:
    error = -degree[0];
    break;
  case CENTER_RIGHT:
  case CENTER_RIGHT + REAR_RIGHT:
    error = degree[2];
    break;
  case CENTER_LEFT:
  case CENTER_LEFT + REAR_LEFT:
    error = -degree[2];
    break;

  // Dual sensor correction
  case FRONT_RIGHT + CENTER_RIGHT:
    error = degree[1];
    //translate(LEFT);
    break;
  case FRONT_LEFT + CENTER_LEFT:
    error = -degree[1];
    //translate(RIGHT);
    break;

  // Translational cases

  case FRONT_RIGHT + REAR_RIGHT:
  case FRONT_RIGHT + CENTER_RIGHT + REAR_RIGHT:
    translate(RIGHT);
    break;
  case FRONT_LEFT + REAR_LEFT:
  case FRONT_LEFT + CENTER_LEFT + REAR_LEFT:
    translate(LEFT);
    break;

  //Rotational cases
  case REAR_RIGHT+FRONT_LEFT:
  case REAR_RIGHT+CENTER_LEFT:
    //rotate(LEFT);
    error = -degree[1];
          break;
  case REAR_LEFT+FRONT_RIGHT:
  case REAR_LEFT+CENTER_RIGHT:
    //rotate(RIGHT);
    error = degree[1];//[2]
          break;
  case FRONT_LEFT + CENTER_LEFT + REAR_LEFT +
       FRONT_RIGHT + CENTER_RIGHT + REAR_RIGHT:
    whiteTape = !whiteTape;

  default:
    error = 0;
    break;
  }

  //Error Continuation
  if ((sensors == 0 || sensors == REAR_RIGHT || sensors == REAR_LEFT + CENTER_LEFT)
    && !(sensors & FRONT_RIGHT) && lastError == degree[2])
        error = degree[2];

  if ((sensors == 0  || sensors == REAR_LEFT || sensors == REAR_LEFT + CENTER_LEFT)
    && !(sensors & FRONT_LEFT) && lastError == -degree[2])
        error = -degree[2];

  printCount++;
  if (printCount == 10) {
    printDiagnostics(sensors);
    printCount = 0;
  }
  PD(error);
}

int sensorDetection() {
  int val = 0;
  if (analogRead(LEFT_FRONT) < threshold)
    val += FRONT_LEFT;
  if (analogRead(RIGHT_FRONT) < threshold)
    val += FRONT_RIGHT;
  if (analogRead(LEFT_CENTER) < threshold)
    val += CENTER_LEFT;
  if (analogRead(RIGHT_CENTER) < threshold)
    val += CENTER_RIGHT;
  if (analogRead(LEFT_REAR) < threshold)
    val += REAR_LEFT;
  if (analogRead(RIGHT_REAR) < threshold)
    val += REAR_RIGHT;

  if (!whiteTape)
  	 val ^= 0x3F;

  return val;
}

void printDiagnostics(int sensors) {
  printf("\n");
  printf("______________________________\n");
  printf("******CONTROL VARIABLES******\n");
	printf("Sensors: %x\n", sensors);
	printf("Whitetape: %d\n", whiteTape);
  printf("Error: %d\n", error);
  printf("lastError: %d\n", lastError);
  printf("********SENSOR VALUES********\n");
	printf("Front Right: %d\n", analogRead(RIGHT_FRONT));
	printf("Center Right: %d\n", analogRead(RIGHT_CENTER));
	printf("Rear Right: %d\n", analogRead(RIGHT_REAR));
	printf("Front Left: %d\n", analogRead(LEFT_FRONT));
	printf("Center Left: %d\n", analogRead(LEFT_CENTER));
	printf("Rear Left: %d\n", analogRead(LEFT_REAR));
  printf("************MOTORS***********\n");
  printf("Left Front: %d\n", getLeftFront());
  printf("Left Rear: %d\n", getLeftRear());
  printf("Right Front: %d\n", getRightFront());
  printf("Right Rear: %d\n", getRightRear());
}

void translate(int right) {
  printf("Translating! :O\n");
  int detect = sensorDetection();

  //Continue transition while front left/right are on line
  while(detect & FRONT_LEFT ||
        detect & FRONT_RIGHT) {
    if (!right)
      tMove(baseSpeed, LEFT);
    else
      tMove(baseSpeed, RIGHT);

    detect = sensorDetection();
  }
}


void rotate(int right) {
  //while(sensorDetection())
  aTurn(right, baseSpeed, baseSpeed);
    printf("Rotating.. d-_-b\n");
    /*
    int detect = sensorDetection();

    while(detect & (REAR_RIGHT+FRONT_LEFT) || detect & (REAR_RIGHT+CENTER_LEFT) ||
         detect & (REAR_LEFT+FRONT_RIGHT) || detect & (REAR_LEFT+CENTER_RIGHT))
    {
      if (right)
        aTurn(right, baseSpeed, baseSpeed);
        else
        aTurn(LEFT, baseSpeed, baseSpeed);
        detect = sensorDetection();
    }*/
}


/*
 //Detects if a right angle has been encountered.
 //If it has, deploy rotation.
 void rightAngleDetection()
 {
        //Right Angle Correction
                if ((analogRead(FRONT_RIGHT) < threshold &&
 analogRead(FRONT_CENTER) <
                        threshold) || (analogRead(REAR_CENTER) < threshold &&
 analogRead(REAR_RIGHT) < threshold))
                                rightAngle(1);
                if ((analogRead(FRONT_RIGHT) < threshold &&
 analogRead(FRONT_CENTER) <
                                threshold) || (analogRead(REAR_CENTER) <
 threshold && analogRead(REAR_RIGHT) < threshold))
                                rightAngle(1);

                if (analogRead(FRONT_LEFT) < threshold &&
 analogRead(FRONT_CENTER) <
                        threshold || analogRead(REAR_LEFT) < threshold &&
 analogRead(REAR_CENTER))
                                rightAngle(0);
                if (analogRead(FRONT_LEFT) < threshold &&
 analogRead(FRONT_CENTER) <
                                threshold || analogRead(REAR_LEFT) < threshold
 && analogRead(REAR_CENTER))
                                rightAngle(0);
        }
*/

void PD(int error) {
  float derivative = 0;
  int highBound = 127; // 110
  int lowBound = -127;
  int motorSpeed = 0;

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

  motorSpeed = (KP * error) + (KD * derivative);

  if (abs(error) == 700) {
    rightMotorSet(-motorSpeed);
    leftMotorSet(motorSpeed);
  }
  else {
    rightMotorSet((baseSpeed - motorSpeed));
    leftMotorSet((baseSpeed + motorSpeed));
  }

  // int leftSpeed = baseSpeed + motorSpeed;
  // int rightSpeed = baseSpeed - motorSpeed;

  if (error != 0) { //&& printCount == 10) {
    //printf("Proposed Right: %d\n", (baseSpeed - motorSpeed));
    //printf("Proposed Left: %d\n\n", (baseSpeed + motorSpeed));
  }

  // Motor Bound Check
  if (getRightFront() < lowBound)
    rightMotorSet(lowBound);
  if (getLeftFront() < lowBound)
    leftMotorSet(lowBound);
  if (getRightRear() > highBound)
    rightMotorSet(highBound);
  if (getLeftRear() > highBound)
    leftMotorSet(highBound);
}

/*
//Checks to see if the back left or right sensor is the only sensor being
detected.
//If so, commence rotation.
int checkOnlySensor(int sensor)
{
        switch(sensor) {
                case 3: if (analogRead(REAR_LEFT) < threshold &&
analogRead(REAR_RIGHT) > threshold &&
                                                                analogRead(REAR_CENTER)
> threshold && analogRead(FRONT_LEFT) > threshold &&
                                                                analogRead(FRONT_CENTER)
> threshold && analogRead(FRONT_RIGHT) > threshold)
                                                                return 1;
                case 4: if (analogRead(REAR_RIGHT) < threshold &&
analogRead(REAR_LEFT) > threshold &&
                                                                analogRead(REAR_CENTER)
> threshold && analogRead(FRONT_LEFT) > threshold &&
                                                                analogRead(FRONT_CENTER)
> threshold && analogRead(FRONT_RIGHT) > threshold)
                                                                return 1;
                return 0;
        }
        return 0;
}*/

// A wild right angle has appeared.
// Bartholemew used rotation.
// It's super effective! <- lol
void rightAngle(int right) {
  // Peform rotation until Bart is aligned with the line
  // This is a comment
  if (right) {
    // while (analogRead(RIGHT_FRONT) > threshold)
    rotate(RIGHT);
  } else {
    // while (analogRead(LEFT_FRONT) > threshold)
    rotate(LEFT);
  }

  // lastTime = millis();
}

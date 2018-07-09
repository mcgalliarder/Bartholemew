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

/**
 * Detects where the line is in relation to the robot and
 * returns a number that reflects that error.
 * If it detects a right angle, it calls the rightAngle function,
 * else it calls the Proportional-Derivative (PD) function
 */
void errorCorrection();

/**
 * Translates until line is found
 * @param right(1) or left(0) translation
 */
void translate(int right);


/**
 * Proportional-Derivative Control
 * Corrects motor speeds on both motors depending on
 * how large the error is, and the time it took for errors to change.
 */
void PD(int error);

/**
 * @return which sensors are hitting the line
 */
int sensorDetection();

/**
 * Prints sensor values and control variables
 * @param sensorDetection variable
 */
void printDiagnostics(int sensors);

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

void lineTrack() {
  int track = true;
  while (track) {
    if (joystickGetDigital(CONTROLLER, RIGHT_BUTT_SET, JOY_DOWN)) {
      track = false;
      delay(500);
    }
    errorCorrection();
  }
}

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
    break;
  case FRONT_LEFT + CENTER_LEFT:
    error = -degree[1];
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
    error = -degree[1];
          break;
  case REAR_LEFT+FRONT_RIGHT:
  case REAR_LEFT+CENTER_RIGHT:
    error = degree[1];
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

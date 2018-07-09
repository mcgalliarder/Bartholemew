#ifndef _LINETRACK_H_
#define _LINETRACK_H_

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

/**
 * Detects where the line is in relation to the robot and
 * returns a number that reflects that error.
 * If it detects a right angle, it calls the rightAngle function,
 * else it calls the Proportional-Derivative (PD) function
 */
void errorCorrection();

/**
 * Proportional-Derivative Control
 * Corrects motor speeds on both motors depending on
 * how large the error is, and the time it took for errors to change.
 */
void PD(int error);

int sensorDetection();

/**
 * Prints sensor values and control variables
 * @param sensorDetection variable
 */
void printDiagnostics(int sensors);

void translate(int right);

void rotate(int right);

int frontCorrection();

int rearCorrection();

void rightAngleDetection();

/**
 * Corrects the robot in the event of a right angle
 */
void rightAngle(int right);


#endif //_LINETRACK_H_

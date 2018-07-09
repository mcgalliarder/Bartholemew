#ifndef _CHASSIS_H_
#define _CHASSIS_H_

/**
 * Sets both drive wheels to certain speeds
 * @param left drive speed
 * @param right drive speed
 */
void chassisSet(int leftFrontSpeed, int leftRearSpeed,
                int rightFrontSpeed, int rightRearSpeed);


/**
 * Turns using only rear motors
 */
void rearTurn(int speed);


/**
 * Rear turns to specified degree
 */
void rearTurnDegree(int speed, int degree);

/**
 * Turns the robot on axis 90 degrees
 * @param speed to turn
 * @param right or left turn
 */
 void aTurn(int right, int leftSpeed, int rightSpeed);

 void aTurnDegree(int leftSpeed, int rightSpeed, int degree);

 void tMove(int speed, int direction);

 void traversal(int speed, int right);

 void traversalCm(int speed, int centimeters);


 void encTraversal(int speed, int right, int encVal);

 /**
  * @return if encoders are working
  */
 int checkEncoders(unsigned int checkTime);

/**
 * Resets the motor encoders to 0
 * @return if reset was successful
 */
 int resetEncoders();

 /**
  * Sets the right motor set to a certain speed
  * @param speed to set
  */
 void rightMotorSet(int speed);

 /**
  * Sets the left motor speed to a certain speed
  * @param speed to set
  */
 void leftMotorSet(int speed);

/**
 * Sets the left drive motor to a certain speed
 * @param speed to set
 */
void leftFrontSet(int speed);

/**
 * Sets the left drive motor to a certain speed
 * @param speed to set
 */
void leftRearSet(int speed);

/**
 * Sets the right drive motor to a certain speed
 * @param speed to set
 */
void rightFrontSet(int speed);

/**
 * Sets the right drive motor to a certain speed
 * @param speed to set
 */
void rightRearSet(int speed);

/**
 * @return speed within acceptable bounds
 */
 int boundCheck(int speed);

/**
 * @return right motor speed
 */
int getRightFront();

/**
 * @return right motor speed
 */
int getRightRear();

/**
 * @return left motor speed
 */
int getLeftFront();

/**
 * @return left motor speed
 */
int getLeftRear();

#endif // _CHASSIS_H_

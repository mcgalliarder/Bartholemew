#ifndef _ARM_H_
#define _ARM_H_

void initScanPosition();

void setAllAxis(int basePos, int midPos, int topPos);

void stopArm();

/**
 * Sets motor speed for the mid axis point of arm
 * @param speed to set
 */
void setMid(int speed);

int setMidAxis(int value, int speed);

/**
 * Sets motor speed for the top axis point of arm
 * @param speed to set
 */
void setTop(int speed);

int setTopAxis(int value);

void basePosition();
/**
 * Sets motor speed for the base axis point of arm
 * @param speed to set
 */
void setBase(int speed);

int setBaseAxis(int value, int speed);


int getMidPot();

int getTopPot();

int getBaseEncoder();

void resetBase();


#endif //_ARM_H_

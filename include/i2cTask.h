#ifndef _I2CTASK_H_
#define _I2CTASK_H_

//static void i2cHandler(void* ignore);
void i2cTaskStart();

int getRightIME();
int getLeftIME();
void killTask();

#endif

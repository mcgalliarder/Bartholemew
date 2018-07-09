#ifndef SONAR_H_
#define SONAR_H_

/**
 * Maintains sonar positioning based on other axis points
 */
int adjustSonar();

int manAdjustSonar(int base, int mid);

/**
 * Determines the distance to an object.
 * @param speed to set
 */
int getDistance();

/**
 * Returns a valid distance out of five samples
 */
int getValidDistance();

/**
 * Gets averaged distance from 5 samples
 * Throws away trash values
 */
 int getAveragedDistance();

#endif //SONAR_H_

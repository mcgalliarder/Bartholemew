#ifndef AVOIDANCE_H_
#define AVOIDANCE_H_

/**
 * Step-based avoidance system
 */
void proceduralAvoidance();

void traverseUntilLoss();

void axisTurnUntilFound();

int calcError(int currentDist);

void avoidPD(int error);

void rearTurnOrient(int currentDist);

void avoid();

void correctDistance(int optimumDistance);

void orient(double error, double travDist);

void cornerTrack();

#endif

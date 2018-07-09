#include "main.h"
#include "arm.h"
#include "sonar.h"

/******************************************************************
* Adjusts sonar
******************************************************************/
int adjustSonar() {
  int baseValue = 20*getBaseEncoder() + 2280;
  return (baseValue - 50) - (getMidPot()+60) + 670;
}

/******************************************************************
* Manually adjusts the sonar according to base and mid values
******************************************************************/
int manAdjustSonar(int base, int mid) {
  int baseValue = 20*base + 2280;
  return (baseValue - 50) - (mid+60) + 670;
}

/******************************************************************
* Returns the distance from one sonar reading
******************************************************************/
int getDistance()
{
   if (sonar == NULL)
     printf("Sonar not initialized\n");
   int distance = ultrasonicGet(sonar);
   if (distance == -1)
     return 100000000;

   return distance;
}


int getValidDistance() {
  int sampleLimit = 5;
  for (int i = 0; i < sampleLimit; i++) {
    int dist = getDistance();
    if (dist < 1000000)
      return dist;
    delay(30);
  }
  return 100000000;
}

/******************************************************************
* Returns the average distance over a number of samples
* Used to filter out innacurate values
******************************************************************/
int getAveragedDistance() {
  int samples = 5;
  int sum = 0;
  int samplesTaken = 0;
  int distances[samples];

  //Retrieve samples
  for (int i = 0; i < samples; i++)
    distances[i] = getDistance();

  //Get average
  for (int i = 0; i < samples; i++)
    if (distances[i] < 1000) {
      sum += distances[i];
      samplesTaken++;
      delay(30);
    }
  if (samplesTaken == 0)
    return 1000000000;
  return sum/samplesTaken;
}

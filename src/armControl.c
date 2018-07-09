#include "main.h"
#include "chassis.h"
#include "lineTrack.h"
#include "arm.h"

int control = 1;

void armControl(int go) {
  delay(300);
  int toggle = false;

  //Controls utilize the right button set, left bumpers, and left joystick
  while (go) {
    //Break out of alt contol (RightButton)
    if (joystickGetDigital(control, RIGHT_BUTT_SET, JOY_RIGHT)) {
      stopArm();
      go = false;
      delay(500);
    }

    //Auto-Adjust Sonar Positioning (LeftButton)
    if (joystickGetDigital(control, RIGHT_BUTT_SET, JOY_LEFT))
      setTop(adjustSonar());

    //Toggle scan/base positioning (DownButton)
    if (joystickGetDigital(control, RIGHT_BUTT_SET, JOY_DOWN)) {
      if (toggle)
        basePosition();
      else
        initScanPosition();
      toggle = !toggle;
      delay(500);
    }

    //Print Data (UpButton)
    if (joystickGetDigital(control, RIGHT_BUTT_SET, JOY_UP)) {
        printf("Sonar Read: %d\n", getDistance());
        printf("Base Encoder: %d\n", getBaseEncoder());
        printf("Mid Pot: %d\n", getMidPot());
        printf("Top Pot: %d\n", getTopPot());
        printf("_____________________________\n\n");
        delay(500);
    }

    //Manual Sonar Positioning Control (Right Bumpers)
    if (joystickGetDigital(control, RIGHT_BUMP, JOY_UP))
      setTop(18);
    else if (joystickGetDigital(control, RIGHT_BUMP, JOY_DOWN))
      setTop(-15);
    else
      setTop(7);

    //Manual arm control (LeftJoy)
    int threshold = 25;
    int basePower = 0;
    int midPower = 0;
    if (abs(joystickGetAnalog(control, 3)) > threshold)
      basePower = joystickGetAnalog(control, 3)/2;
    else if (getBaseEncoder() > 50) {
      basePower = -10;
    }
    else basePower = 10;

    if (abs(joystickGetAnalog(control, 4)) > threshold)
      midPower = joystickGetAnalog(control, 4)/2;
    else midPower = 10;

    if (getBaseEncoder() < 5 && basePower < 0)
      setBase(10);
    else
      setBase(basePower);
    if (getMidPot() < 710 && midPower < 0)
      setMid(10);
    else
      setMid(midPower);

    delay(20);
  }
}

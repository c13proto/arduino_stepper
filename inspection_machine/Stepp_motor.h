// Stepp_motor.h

#ifndef _STEPP_MOTOR_h
#define _STEPP_MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <MultiStepper.h>
#include <AccelStepper.h>

#define LIMIT_Z 11
#define LIMIT_Y 10
#define LIMIT_X 9
#define DIR_Z 7
#define DIR_Y 6
#define DIR_X 5
#define STEP_Z 4
#define STEP_Y 3
#define STEP_X 2
#define MOTOR_ENABLE 8//これをLOWにしないとモタドラ駆動しない
#define DRIVER_ON digitalWrite(MOTOR_ENABLE, LOW)//これをLOWにしないとモタドラ駆動しない
#define DRIVER_OFF digitalWrite(MOTOR_ENABLE, HIGH)//これをLOWにしないとモタドラ駆動しない


class Stepp_motorClass
{
 protected:


 public:
	void init();
};
extern AccelStepper motorX;
extern AccelStepper motorY;
extern AccelStepper motorZ;
extern Stepp_motorClass Stepp_motor;

#endif


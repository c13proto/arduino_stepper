// 
// 
// 
#include "Stepp_motor.h"

AccelStepper motorX;
AccelStepper motorY;
AccelStepper motorZ;

void Stepp_motorClass::init()
{
	pinMode(MOTOR_ENABLE, OUTPUT);
	pinMode(DIR_X, OUTPUT);	
	pinMode(DIR_Y, OUTPUT);
	pinMode(DIR_Z, OUTPUT);
	pinMode(STEP_X, OUTPUT);
	pinMode(STEP_Y, OUTPUT);
	pinMode(STEP_Z, OUTPUT);

	DRIVER_OFF;

	motorX = AccelStepper(AccelStepper::DRIVER, STEP_X, DIR_X);
	motorY = AccelStepper(AccelStepper::DRIVER, STEP_Y, DIR_Y);
	motorZ = AccelStepper(AccelStepper::DRIVER, STEP_Z, DIR_Z);
	
	motorZ.setMaxSpeed((float)1000);//[puls/sec] accelstepperÇ≈ÇÕ1000ÇÕí¥Ç¶Ç»Ç¢ê›åvÇÇµÇƒÇ¢ÇÈÇÁÇµÇ¢
	motorZ.setSpeed((float)1000);
	motorZ.setAcceleration((float)500);
	motorZ.moveTo(200*32);
}


Stepp_motorClass Stepp_motor;


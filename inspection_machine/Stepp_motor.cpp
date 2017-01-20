// 
// 
// 
#include "Stepp_motor.h"
#include "keypad.h"
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
	

	speed_z = 1000;
	speed_x = 1000;
	speed_y = 1000;
	motorX.setMaxSpeed((float)1000);//[puls/sec] accelstepperÇ≈ÇÕ1000ÇÕí¥Ç¶Ç»Ç¢ê›åvÇÇµÇƒÇ¢ÇÈÇÁÇµÇ¢(ê≥ÇÃêîÇë„ì¸)
	motorY.setMaxSpeed((float)1000);
	motorZ.setMaxSpeed((float)1000);
	motorX.setSpeed(speed_x);
	motorY.setSpeed(speed_y);
	motorZ.setSpeed(speed_z);
	motorX.setAcceleration((float)500);
	motorY.setAcceleration((float)500);
	motorZ.setAcceleration((float)500);

}

void Stepp_motorClass::stepp_master_ctrl()
{
	X_master_ctrl();//4,6Ç⁄ÇΩÇÒ
	Y_master_ctrl();//2,8Ç⁄ÇΩÇÒ
	Z_master_ctrl();//1,3Ç⁄ÇΩÇÒ
}
void Stepp_motorClass::X_master_ctrl()
{
	if (PAD_6 != 0)
	{
		if (speed_x < 0)speed_x*=-1;
		motorX.setSpeed(speed_x);
		motorX.runSpeed();
	}
	if (PAD_4 != 0)
	{
		if (speed_x > 0)speed_x *= -1;
		motorX.setSpeed(speed_x);
		motorX.runSpeed();
	}
}
void Stepp_motorClass::Y_master_ctrl()
{
	if (PAD_2 != 0)
	{
		if (speed_y < 0)speed_y *= -1;
		motorY.setSpeed(speed_y);
		motorY.runSpeed();
	}
	if (PAD_8 != 0)
	{
		if (speed_y > 0)speed_y *= -1;
		motorY.setSpeed(speed_y);
		motorY.runSpeed();
	}
}
void Stepp_motorClass::Z_master_ctrl()
{
	if (PAD_3 != 0)
	{
		if (speed_z < 0)speed_z *= -1;
		motorZ.setSpeed(speed_z);
		motorZ.runSpeed();
	}
	if (PAD_1 != 0)
	{
		if (speed_z > 0)speed_z *= -1;
		motorZ.setSpeed(speed_z);
		motorZ.runSpeed();
	}
}
void Stepp_motorClass::stepp_slave_ctrl(String command)
{
	//Vx:XÇÃë¨ìxê›íË
	//X:xÇ…à⁄ìÆ
	if (command.startsWith("Vz"))
	{
		//string.toFloat()
		String val = command;
		val.replace("Vz", "");
		speed_z = (int)(val.toFloat());
		motorZ.setMaxSpeed(speed_z);
		motorZ.setSpeed(speed_z);
	}
	if (command.startsWith("Z"))
	{
		//string.toFloat()
		String val = command;
		val.replace("Z", "");
		aim_z = (int)(val.toFloat());
		motorZ.moveTo(aim_z);
		motorZ.run();
	}

}

Stepp_motorClass Stepp_motor;


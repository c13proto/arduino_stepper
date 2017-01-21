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
//���^�h���V�[���h��PIN�z�u
#define LIMIT_Z 11
#define LIMIT_Y 10
#define LIMIT_X 9
#define DIR_Z 7
#define DIR_Y 6
#define DIR_X 5
#define STEP_Z 4
#define STEP_Y 3
#define STEP_X 2
#define MOTOR_ENABLE 8//�����LOW�ɂ��Ȃ��ƃ��^�h���쓮���Ȃ�
#define DRIVER_ON digitalWrite(MOTOR_ENABLE, LOW)
#define DRIVER_OFF digitalWrite(MOTOR_ENABLE, HIGH)


class Stepp_motorClass
{
 protected://�N���X���ŌĂяo�����
	 void X_master_ctrl();
	 void Y_master_ctrl();
	 void Z_master_ctrl();
	 void X_slave_ctrl(String);
	 void Y_slave_ctrl(String);
	 void Z_slave_ctrl(String);
	 boolean isFloat(String);
	 int speed_x;
	 int speed_y;
	 int speed_z;


 public://���C���ŌĂяo�����
	void init();
	void stepp_master_ctrl();
	void stepp_slave_ctrl(String);
	void stepp_stop();
};
extern AccelStepper motorX;
extern AccelStepper motorY;
extern AccelStepper motorZ;

extern Stepp_motorClass Stepp_motor;

#endif


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

extern AccelStepper MOTOR_X;
extern AccelStepper MOTOR_Y;
extern AccelStepper MOTOR_Z;

extern int SET_POS_MODE;//master_ctrl�Őݒ肵�Ă��郂�[�^(x,y,z)=(1,2,3)
extern String MOTOR_POS_SET;

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
	 void motors_run();


 public://���C���ŌĂяo�����
	void init();
	void stepp_master_ctrl();
	void stepp_slave_ctrl(String);
	void motors_stop();
};


extern Stepp_motorClass Stepp_motor;

#endif


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
#define STEP_A	12//SpnEn
#define DIR_A	13//SpnDr
#define LIMIT_Z 11
#define LIMIT_Y 10
#define LIMIT_X 9
#define DIR_Z	7
#define DIR_Y	6
#define DIR_X	5
#define STEP_Z	4
#define STEP_Y	3
#define STEP_X	2
#define MOTOR_XY_ENABLE		8//�����LOW�ɂ��Ȃ��ƃ��^�h���쓮���Ȃ� PH5
#define MOTOR_Z_ENABLE		A3//�{����CoolantEnable PF3
#define ENABLE_SW			A0//�{����Abort PF0
#define DRIVER_XY_ON	PORTH &= ~_BV(5)//���荞�ݎ��ɖ���Ă΂��̂ł��̏�����
#define DRIVER_XY_OFF	PORTH |= _BV(5)
#define DRIVER_Z_ON		PORTF &= ~_BV(3)
#define DRIVER_Z_OFF	PORTF |= _BV(3)
#define SW_STATE	PINF & _BV(0)//mode_update()��MOTOR_XY_ENABLE�𐧌䂷��
extern AccelStepper MOTOR_X;
extern AccelStepper MOTOR_Y;
extern AccelStepper MOTOR_Z;
extern int			SET_POS_MODE;//master_ctrl�Őݒ肵�Ă��郂�[�^
extern String		MOTOR_POS_SET;
extern int		DRIVER_STATE;//

class Stepp_motorClass
{
 protected://�N���X���ŌĂяo�����
	 void X_master_input_ctrl();
	 void Y_master_input_ctrl();
	 void Z_master_input_ctrl();
	 void master_pad_controller_ctrl(int,int);
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


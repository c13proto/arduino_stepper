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
#define PIN_DIR_A		13//SpnDr
#define PIN_BUZZER		12//�{����STEP_A or SpnEn
#define PIN_LIMIT_X1	11//PB5 �{����LIMIT_Z
#define PIN_LIMIT_Y0	10//y�Ⴂ�����~�b�g(timer�Ɏg�p���Ă��邽��pwm�o�͕s��)�@PB4
#define PIN_LIMIT_X0	9//x�Ⴂ�����~�b�g(timer�Ɏg�p���Ă��邽��pwm�o�͕s��)�@PH6
#define PIN_ENABLE_XY	8//�����LOW�ɂ��Ȃ��ƃ��^�h���쓮���Ȃ� PH5
#define PIN_DIR_Z		7
#define PIN_DIR_Y		6	
#define PIN_DIR_X		5
#define PIN_STEP_Z		4
#define PIN_STEP_Y		3
#define PIN_STEP_X		2
#define PIN_XY_SW		A0//�{����Abort PF0
#define PIN_LIMIT_Y1 	A1//�{����Hold PF1
#define PIN_ENABLE_Z	A3//�{����CoolantEnable PF3

//Output-High/Output-Low/Read High or Low
#define DRIVER_XY_ON	cbi(PORTH,5)//
#define DRIVER_XY_OFF	sbi(PORTH,5)//
#define DRIVER_Z_ON		cbi(PORTF,3)//
#define DRIVER_Z_OFF	sbi(PORTF,3)//
#define SLIDE_SW	ibi(PINF,0)//�X���C�h�X�C�b�`�Dmode_update()��PIN_ENABLE_XY�𐧌䂷��
#define LIMIT_X0	ibi(PINH,6)//���~�b�g�X�C�b�`
#define LIMIT_Y0	ibi(PINB,4)//
#define LIMIT_X1	ibi(PINB,5)//
#define LIMIT_Y1	ibi(PINF,1)//

//�O���[�o���ϐ���Define
extern int AD4;
extern AccelStepper MOTOR_X;
extern AccelStepper MOTOR_Y;
extern AccelStepper MOTOR_Z;
extern int CTRL_MODE;
#define MODE_SLEEP 0
#define MODE_MASTER 1
#define MODE_SLAVE 2
extern int SET_POS_MODE;//master_ctrl�Őݒ肵�Ă��郂�[�^
#define MODE_X 1
#define MODE_Y 2
#define MODE_Z 3
extern String	MOTOR_POS_SET;
extern int		DRIVER_STATE;//
#define STATE_OFF_OFF	0//xy=off z=off
#define STATE_OFF_ON	1
#define STATE_ON_ON		2
extern bool COMMAND_RECIEVE_FLAG;
extern String SLAVE_COMMAND;
#define SLAVE_COMMAND_LENGTH 10

//�p���X�̕����P�ʁ@���Z
#define DEG_TO_PULSE(DEG)		(long)((double)DEG*17.7777778)//=deg*6400/360
#define PULSE_TO_DEG(PULSE)		(double)PULSE*0.05625//pulse*360.0/6400.0;
#define mm_TO_PULSE(mm)			(long)((double)mm*100.0)// mm*400.0(=200[pulse/1��]]*2[����])/4.0(=1��]������̐i�ދ���[mm])
#define PULSE_TO_mm(PULSE)		(double)PULSE*0.01//pulse*4.0 / 400.0

class Stepp_motorClass
{
 private://�N���X���ŌĂяo��
	 void X_master_input_ctrl();
	 void Y_master_input_ctrl();
	 void Z_master_input_ctrl();
	 void master_pad_controller_ctrl(int,int);
	 void command_update();
	 void X_slave_ctrl(String);
	 void Y_slave_ctrl(String);
	 void Z_slave_ctrl(String);
	 bool isFloat(String);
	 void motors_run();
	 void limit_ctrl();
	 void motors_stop();

 public://���C���ŌĂяo��
	void init();
	void master_ctrl();
	void slave_ctrl();
	void mode_changed_ctrl();
};


extern Stepp_motorClass Stepp_motor;

#endif


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
//モタドラシールドのPIN配置
#define PIN_STEP_A		12//SpnEn
#define PIN_DIR_A		13//SpnDr
#define PIN_LIMIT_X1	11//PB5 本来はLIMIT_Z
#define PIN_LIMIT_Y0	10//PB4
#define PIN_LIMIT_X0	9//PH6
#define PIN_ENABLE_XY	8//これをLOWにしないとモタドラ駆動しない PH5
#define PIN_DIR_Z		7
#define PIN_DIR_Y		6	
#define PIN_DIR_X		5
#define PIN_STEP_Z		4
#define PIN_STEP_Y		3
#define PIN_STEP_X		2
#define PIN_SLIDE_SW	A0//本来はAbort PF0
#define PIN_LIMIT_Y1 	A1//本来はHold PF1
#define PIN_ENABLE_Z	A3//本来はCoolantEnable PF3

#define DRIVER_XY_ON	cbi(PORTH,5)//割り込み時に毎回呼ばれるのでこの書き方
#define DRIVER_XY_OFF	sbi(PORTH,5)//
#define DRIVER_Z_ON		cbi(PORTF,3)//
#define DRIVER_Z_OFF	sbi(PORTF,3)//
#define SLIDE_SW	ibi(PINF,0)//スライドスイッチ．mode_update()でPIN_ENABLE_XYを制御する
#define LIMIT_X0	ibi(PINH,6)//リミットスイッチ
#define LIMIT_Y0	ibi(PINB,4)//
#define LIMIT_X1	ibi(PINB,5)//
#define LIMIT_Y1	ibi(PINF,1)//

extern AccelStepper MOTOR_X;
extern AccelStepper MOTOR_Y;
extern AccelStepper MOTOR_Z;
extern int			SET_POS_MODE;//master_ctrlで設定しているモータ
#define MODE_X 1
#define MODE_Y 2
#define MODE_Z 3
extern String		MOTOR_POS_SET;
extern int		DRIVER_STATE;//
#define STATE_OFF_OFF	0//xy=off z=off
#define STATE_OFF_ON	1
#define STATE_ON_ON		2
class Stepp_motorClass
{
 protected://クラス内で呼び出すやつ
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
	 void limit_ctrl();

 public://メインで呼び出すやつ
	void init();
	void stepp_master_ctrl();
	void stepp_slave_ctrl(String);
	void motors_stop();
};


extern Stepp_motorClass Stepp_motor;

#endif


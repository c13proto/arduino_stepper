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
	DRIVER_OFF;

	pinMode(DIR_X, OUTPUT);	
	pinMode(DIR_Y, OUTPUT);
	pinMode(DIR_Z, OUTPUT);
	pinMode(STEP_X, OUTPUT);
	pinMode(STEP_Y, OUTPUT);
	pinMode(STEP_Z, OUTPUT);

	motorX = AccelStepper(AccelStepper::DRIVER, STEP_X, DIR_X);
	motorY = AccelStepper(AccelStepper::DRIVER, STEP_Y, DIR_Y);
	motorZ = AccelStepper(AccelStepper::DRIVER, STEP_Z, DIR_Z);
	
	//runSpeed()で方向反転する時に使う
	speed_z = 1000;
	speed_x = 1000;
	speed_y = 1000;

	motorX.setMaxSpeed(speed_x);//[puls/sec] accelstepperでは1000は超えない設計をしているらしい(正の数を代入)
	motorY.setMaxSpeed(speed_y);//1000以上の数を入れると変な動作するときあり
	motorZ.setMaxSpeed(speed_z);
	motorX.setSpeed(speed_x);
	motorY.setSpeed(speed_y);
	motorZ.setSpeed(speed_z);
	motorX.setAcceleration(1000);
	motorY.setAcceleration(1000);
	motorZ.setAcceleration(1000);

}

void Stepp_motorClass::stepp_master_ctrl()
{
	X_master_ctrl();//4,6ぼたん
	Y_master_ctrl();//2,8ぼたん
	Z_master_ctrl();//1,3ぼたん
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
	static String command_old = "";

	if (!command.equals(command_old))//コマンドが同じ場合はスキップ(これやらないとクソ重かった)
	{
		String command_trim = command;
		command_trim.replace(" ", "");//スペースなくす
		X_slave_ctrl(command_trim);
		Y_slave_ctrl(command_trim);
		Z_slave_ctrl(command_trim);
	}
	command_old = command;

	motorX.run();
	motorY.run();
	motorZ.run();

}
void Stepp_motorClass::X_slave_ctrl(String command)
{
	if (command.startsWith("Vx"))
	{
		String val = command;
		val.replace("Vx", "");
		if (isFloat(val)) motorX.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("X"))
	{
		String val = command;
		val.replace("X", "");
		if (isFloat(val)) motorX.moveTo(val.toInt());//目標位置の指定．motorX.run()で動く
	}
	if (command.equals("STOP_X") || command.equals("STOP"))motorX.stop();//止まる位置に目標座標を変更


}
void Stepp_motorClass::Y_slave_ctrl(String command)
{
	if (command.startsWith("Vy"))
	{
		String val = command;
		val.replace("Vy", "");
		if (isFloat(val)) motorY.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("Y"))
	{
		String val = command;
		val.replace("Y", "");
		if (isFloat(val)) motorY.moveTo(val.toInt());
	}
	if (command.equals("STOP_Y") || command.equals("STOP"))motorY.stop();


}
void Stepp_motorClass::Z_slave_ctrl(String command)
{
	if (command.startsWith("Vz"))
	{
		String val = command;
		val.replace("Vz", "");
		if (isFloat(val)) motorZ.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("Z"))
	{
		String val = command;
		val.replace("Z", "");
		if (isFloat(val)) motorZ.moveTo(val.toInt());
	}
	if (command.equals("STOP_Z") || command.equals("STOP"))motorZ.stop();


}
boolean Stepp_motorClass::isFloat(String tString) {//Stringを数値化出来るかどうか
	String tBuf;
	boolean decPt = false;

	if (tString.charAt(0) == '+' || tString.charAt(0) == '-') tBuf = &tString[1];
	else tBuf = tString;

	for (int x = 0; x<tBuf.length(); x++)
	{
		if (tBuf.charAt(x) == '.') {
			if (decPt) return false;
			else decPt = true;
		}
		else if (tBuf.charAt(x) < '0' || tBuf.charAt(x) > '9') return false;
	}
	return true;
}
void Stepp_motorClass::stepp_stop()
{
	motorX.move(0);//急停止になるので動いてるときにやらないほうがいい
	motorY.move(0);
	motorZ.move(0);
	motorX.stop();
	motorY.stop();
	motorZ.stop();
}
Stepp_motorClass Stepp_motor;


// 
// 
// 
#include "Stepp_motor.h"
#include "keypad.h"
AccelStepper MOTOR_X;
AccelStepper MOTOR_Y;
AccelStepper MOTOR_Z;

int SET_POS_MODE;//master_ctrlで設定しているモータ(x,y,z)=(1,2,3)
String MOTOR_POS_SET;


void Stepp_motorClass::init()
{
	Serial.println("Stepp_motorClass.init()");

	pinMode(MOTOR_ENABLE, OUTPUT);
	DRIVER_OFF;

	pinMode(DIR_X, OUTPUT);	
	pinMode(DIR_Y, OUTPUT);
	pinMode(DIR_Z, OUTPUT);
	pinMode(STEP_X, OUTPUT);
	pinMode(STEP_Y, OUTPUT);
	pinMode(STEP_Z, OUTPUT);

	MOTOR_X = AccelStepper(AccelStepper::DRIVER, STEP_X, DIR_X);
	MOTOR_Y = AccelStepper(AccelStepper::DRIVER, STEP_Y, DIR_Y);
	MOTOR_Z = AccelStepper(AccelStepper::DRIVER, STEP_Z, DIR_Z);
	
	//runSpeed()で方向反転する時に使う
	speed_z = 1000;
	speed_x = 1000;
	speed_y = 1000;
	SET_POS_MODE = 0;//master_ctrlで設定しているモータx:1 y:2 z:3
	MOTOR_POS_SET = "";

	MOTOR_X.setMaxSpeed(speed_x);//[puls/sec] accelstepperでは1000は超えない設計をしているらしい(正の数を代入)
	MOTOR_Y.setMaxSpeed(speed_y);//1000以上の数を入れると変な動作するときあり
	MOTOR_Z.setMaxSpeed(speed_z);
	MOTOR_X.setSpeed(speed_x);
	MOTOR_Y.setSpeed(speed_y);
	MOTOR_Z.setSpeed(speed_z);
	MOTOR_X.setAcceleration(1000);
	MOTOR_Y.setAcceleration(1000);
	MOTOR_Z.setAcceleration(1000);
	MOTOR_X.moveTo(0);
	MOTOR_Y.moveTo(0);
	MOTOR_Z.moveTo(0);

}

void Stepp_motorClass::stepp_master_ctrl()
{
	X_master_ctrl();
	Y_master_ctrl();
	Z_master_ctrl();

	motors_run();
}
void Stepp_motorClass::X_master_ctrl()
{
	if (SET_POS_MODE != 1 && PRESSED_KEY == 'A')
		SET_POS_MODE = 1;
	else if (SET_POS_MODE == 1)
	{
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//数字の入力
		else if (PRESSED_KEY == '*')//+-の入力
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-", "");
			else MOTOR_POS_SET = "-" + MOTOR_POS_SET;
		}
		else if (PRESSED_KEY == '#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length() - 1);//文字訂正
		else if (PRESSED_KEY == 'A')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if (MOTOR_POS_SET.length() != 0)
					MOTOR_X.moveTo(MOTOR_POS_SET.toInt());//空文字じゃなければ
				else MOTOR_X.move(0);//空文字なら動かない
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Y_master_ctrl()
{
	if (SET_POS_MODE != 2 && PRESSED_KEY == 'B')
		SET_POS_MODE = 2;
	else if (SET_POS_MODE == 2)
	{
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//数字の入力
		else if (PRESSED_KEY == '*')//+-の入力
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-", "");
			else MOTOR_POS_SET = "-" + MOTOR_POS_SET;
		}
		else if (PRESSED_KEY == '#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length() - 1);//文字訂正
		else if (PRESSED_KEY == 'B')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if (MOTOR_POS_SET.length() != 0)
					MOTOR_Y.moveTo(MOTOR_POS_SET.toInt());//空文字じゃなければ
				else MOTOR_Y.move(0);//空文字なら動かない
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Z_master_ctrl()
{
	if (SET_POS_MODE != 3 && PRESSED_KEY=='C')
		SET_POS_MODE = 3;
	else if (SET_POS_MODE == 3)
	{	
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//数字の入力
		else if (PRESSED_KEY == '*')//+-の入力
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-","");
			else MOTOR_POS_SET= "-"+MOTOR_POS_SET;
		}
		else if (PRESSED_KEY=='#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length()-1);//文字訂正
		else if (PRESSED_KEY=='C')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if(MOTOR_POS_SET.length() != 0)
					MOTOR_Z.moveTo(MOTOR_POS_SET.toInt());//空文字じゃなければ
				else MOTOR_Z.move(0);//空文字なら動かない
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
	//if (PRESSED_KEY != '\0')Serial.print(MOTOR_POS_SET);

	//if (PAD_3 != 0)
	//{
	//	if (speed_z < 0)speed_z *= -1;
	//	MOTOR_Z.setSpeed(speed_z);
	//	MOTOR_Z.runSpeed();
	//}
	//if (PAD_1 != 0)
	//{
	//	if (speed_z > 0)speed_z *= -1;
	//	MOTOR_Z.setSpeed(speed_z);
	//	MOTOR_Z.runSpeed();
	//}
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

	motors_run();

}
void Stepp_motorClass::X_slave_ctrl(String command)
{
	if (command.startsWith("Vx"))
	{
		String val = command;
		val.replace("Vx", "");
		if (isFloat(val)) MOTOR_X.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("X"))
	{
		String val = command;
		val.replace("X", "");
		if (isFloat(val)) MOTOR_X.moveTo(val.toInt());//目標位置の指定．MOTOR_X.run()で動く
	}
	if (command.equals("STOP_X") || command.equals("STOP"))MOTOR_X.stop();//止まる位置に目標座標を変更


}
void Stepp_motorClass::Y_slave_ctrl(String command)
{
	if (command.startsWith("Vy"))
	{
		String val = command;
		val.replace("Vy", "");
		if (isFloat(val)) MOTOR_Y.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("Y"))
	{
		String val = command;
		val.replace("Y", "");
		if (isFloat(val)) MOTOR_Y.moveTo(val.toInt());
	}
	if (command.equals("STOP_Y") || command.equals("STOP"))MOTOR_Y.stop();


}
void Stepp_motorClass::Z_slave_ctrl(String command)
{
	if (command.startsWith("Vz"))
	{
		String val = command;
		val.replace("Vz", "");
		if (isFloat(val)) MOTOR_Z.setMaxSpeed(val.toInt());
	}
	if (command.startsWith("Z"))
	{
		String val = command;
		val.replace("Z", "");
		if (isFloat(val)) MOTOR_Z.moveTo(val.toInt());
	}
	if (command.equals("STOP_Z") || command.equals("STOP"))MOTOR_Z.stop();


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
void Stepp_motorClass::motors_stop()
{
	MOTOR_X.move(0);//急停止になるので動いてるときにやらないほうがいい
	MOTOR_Y.move(0);
	MOTOR_Z.move(0);
}
void Stepp_motorClass::motors_run()
{
	MOTOR_X.run();
	MOTOR_Y.run();
	MOTOR_Z.run();
}

Stepp_motorClass Stepp_motor;


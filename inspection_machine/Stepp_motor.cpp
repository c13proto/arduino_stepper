#include "Stepp_motor.h"
#include "keypad.h"
AccelStepper MOTOR_X;
AccelStepper MOTOR_Y;
AccelStepper MOTOR_Z;
int AD4;
int CTRL_MODE;
int SET_POS_MODE;//master_ctrlで設定しているモータ(x,y,z)=(1,2,3)
String MOTOR_POS_SET;
int DRIVER_STATE;
bool COMMAND_RECIEVE_FLAG;
String SLAVE_COMMAND;
void Stepp_motorClass::init()
{
	pinMode(PIN_ENABLE_XY, OUTPUT);
	pinMode(PIN_ENABLE_Z, OUTPUT);
	DRIVER_XY_OFF;
	DRIVER_Z_OFF;
	pinMode(PIN_SLIDE_SW, INPUT_PULLUP);//xyモータのEnableを制御する
	pinMode(PIN_LIMIT_X0, INPUT_PULLUP);//リミットスイッチ
	pinMode(PIN_LIMIT_Y0, INPUT_PULLUP);//リミットスイッチ
	pinMode(PIN_LIMIT_X1, INPUT_PULLUP);//リミットスイッチ
	pinMode(PIN_LIMIT_Y1, INPUT_PULLUP);//リミットスイッチ

	pinMode(PIN_DIR_X, OUTPUT);	
	pinMode(PIN_DIR_Y, OUTPUT);
	pinMode(PIN_DIR_Z, OUTPUT);
	pinMode(PIN_STEP_X, OUTPUT);
	pinMode(PIN_STEP_Y, OUTPUT);
	pinMode(PIN_STEP_Z, OUTPUT);

	MOTOR_X = AccelStepper(AccelStepper::DRIVER, PIN_STEP_X, PIN_DIR_X);
	MOTOR_Y = AccelStepper(AccelStepper::DRIVER, PIN_STEP_Y, PIN_DIR_Y);
	MOTOR_Z = AccelStepper(AccelStepper::DRIVER, PIN_STEP_Z, PIN_DIR_Z);
	
	CTRL_MODE = MODE_SLEEP;
	SET_POS_MODE = NONE;//master_ctrlで設定しているモータx:1 y:2 z:3
	MOTOR_POS_SET = "";
	DRIVER_STATE = STATE_OFF_OFF;//ドライバを有効にしてるかどうか．z xy= 0:off off 1:on off 2:on on
	COMMAND_RECIEVE_FLAG = false;
	SLAVE_COMMAND="no command";

	MOTOR_X.setMaxSpeed(1000);//[puls/sec] accelstepperでは1000は超えない設計をしているらしい(正の数を代入)
	MOTOR_Y.setMaxSpeed(1000);//1000以上の数を入れると変な動作するときあり
	MOTOR_Z.setMaxSpeed(1000);
	//MOTOR_X.setSpeed(1000);//runSpeed()で出力
	//MOTOR_Y.setSpeed(1000);
	//MOTOR_Z.setSpeed(1000);
	MOTOR_X.setAcceleration(5000);
	MOTOR_Y.setAcceleration(5000);
	MOTOR_Z.setAcceleration(5000);
}

void Stepp_motorClass::master_ctrl()
{
	if (DRIVER_STATE == STATE_ON_ON)
	{
		X_master_input_ctrl();
		Y_master_input_ctrl();
	}
	Z_master_input_ctrl();
	master_pad_controller_ctrl(500,500);

	motors_run();
}
void Stepp_motorClass::master_pad_controller_ctrl(int xy,int z)
{
	if (SET_POS_MODE == 0)
	{
		//同じloopでmove()指定したあとからのstop()が効かないのでリミット条件をここに書いている
		if (DRIVER_STATE == STATE_ON_ON) {
			if (PAD_6 == HOLD && LIMIT_X1)MOTOR_X.move(xy);
			if ((PAD_4 == HOLD || PAD_5 == HOLD) && LIMIT_X0)MOTOR_X.move(-xy);//5ボタンはxyを初期位置に移動
			if (PAD_2 == HOLD && LIMIT_Y1)MOTOR_Y.move(xy);
			if ((PAD_8 == HOLD || PAD_5 == HOLD) && LIMIT_Y0)MOTOR_Y.move(-xy);
		}
		if ( PAD_3 ==HOLD )MOTOR_Z.move(z);
		if  (PAD_1 ==HOLD )MOTOR_Z.move(-z);

		if (PAD_4 == RELEASED || PAD_6 == RELEASED)MOTOR_X.stop();//ボタン離されたら止める
		if (PAD_2 == RELEASED || PAD_8 == RELEASED)MOTOR_Y.stop();
		if (PAD_1 == RELEASED || PAD_3 == RELEASED)MOTOR_Z.stop();
		if (PAD_5 == RELEASED)
		{
			MOTOR_X.stop();
			MOTOR_Y.stop();
		}
	}									  
}
void Stepp_motorClass::X_master_input_ctrl()
{
	if (SET_POS_MODE != MODE_X && PRESSED_KEY == 'A')
		SET_POS_MODE = MODE_X;
	else if (SET_POS_MODE == MODE_X)
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
				if (MOTOR_POS_SET.length() != 0)//空文字じゃなければ
				{
					if (MOTOR_POS_SET.equals("000"))MOTOR_X.setCurrentPosition(0);//000のときのみ現在位置を0にする
					else MOTOR_X.moveTo(MOTOR_POS_SET.toInt());
				}
				else MOTOR_X.stop();//空文字なら動かないor止める
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Y_master_input_ctrl()
{
	if (SET_POS_MODE != MODE_Y && PRESSED_KEY == 'B')
		SET_POS_MODE = MODE_Y;
	else if (SET_POS_MODE == MODE_Y)
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
				if (MOTOR_POS_SET.length() != 0)//空文字じゃなければ
				{
					if (MOTOR_POS_SET.equals("000"))MOTOR_Y.setCurrentPosition(0);//000のときのみ現在位置を0にする
					else MOTOR_Y.moveTo(MOTOR_POS_SET.toInt());
				}
				else MOTOR_Y.stop();//空文字なら動かない
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Z_master_input_ctrl()
{
	if (SET_POS_MODE != MODE_Z && PRESSED_KEY=='C')
		SET_POS_MODE = MODE_Z;
	else if (SET_POS_MODE == MODE_Z)
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
				if (MOTOR_POS_SET.length() != 0)//空文字じゃなければ
				{
					if(MOTOR_POS_SET.equals("000"))MOTOR_Z.setCurrentPosition(0);//000のときのみ現在位置を0にする
					else MOTOR_Z.moveTo(MOTOR_POS_SET.toInt());
				}
				else MOTOR_Z.stop();//空文字なら動かない
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}

void Stepp_motorClass::slave_ctrl()
{
	command_update();
	if (COMMAND_RECIEVE_FLAG)//コマンド受信時のみ動作
	{
		SLAVE_COMMAND.trim();//スペースなくす
		if (DRIVER_STATE == STATE_ON_ON) {
			X_slave_ctrl(SLAVE_COMMAND);
			Y_slave_ctrl(SLAVE_COMMAND);
		}
		Z_slave_ctrl(SLAVE_COMMAND);
	}
	motors_run();
}
void Stepp_motorClass::X_slave_ctrl(String command)
{
	if (command.startsWith("Vx"))
	{
		String val = command;
		val.replace("Vx", "");
		if (isFloat(val))MOTOR_X.setMaxSpeed(val.toInt());//速度変更
	}
	if (command.startsWith("X"))
	{
		String val = command;
		val.replace("X", "");
		if (isFloat(val))MOTOR_X.moveTo(val.toInt());//目標位置の指定．MOTOR_X.run()で動く
	}
	if (command.equals("CLEAR_X"))MOTOR_X.setCurrentPosition(0);//現在位置を0にすると同時に止まる(ライブラリの仕様上)
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
	if (command.equals("CLEAR_Y"))MOTOR_Y.setCurrentPosition(0);
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
	if(command.equals("CLEAR_Z"))MOTOR_Z.setCurrentPosition(0);
	if (command.equals("STOP_Z") || command.equals("STOP"))MOTOR_Z.stop();


}

void Stepp_motorClass::command_update()
{
	static char command_buff[SLAVE_COMMAND_LENGTH];
	static int char_counter = 0;

	if (Serial.available())
	{
		char data = Serial.read();

		if (data != ';') {//11文字目に';'を送られることでコマンドと認識させる
			command_buff[char_counter] = data;
			char_counter++;
		}
		else {
			int i = 0;
			if (char_counter == SLAVE_COMMAND_LENGTH)
			{
				SLAVE_COMMAND = "          ";//COMMAND_LENGTHぶんのスペース用意してやらないと格納してくれないみたい
				for (i = 0; i < SLAVE_COMMAND_LENGTH; i++)SLAVE_COMMAND[i] = command_buff[i];
				COMMAND_RECIEVE_FLAG = true;
			}
			else SLAVE_COMMAND = "no command";
			char_counter = 0;
		}
	}
}
void Stepp_motorClass::mode_changed_ctrl()
{
	static char mode_old;
	if (mode_old != CTRL_MODE)
	{
		Stepp_motor.motors_stop();//モータのポジション保持しない
		SET_POS_MODE = NONE;//ポジションを設定していた場合はクリア
		while (Serial.read() != -1);//モード変更時はシリアルのバッファクリア
		SLAVE_COMMAND = "no command";//コマンドをクリア
	}
	mode_old = CTRL_MODE;
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
	MOTOR_X.move(0);//その場で止まる
	MOTOR_Y.move(0);
	MOTOR_Z.move(0);
}
void Stepp_motorClass::motors_run()
{
	limit_ctrl();
	MOTOR_X.run();
	MOTOR_Y.run();
	MOTOR_Z.run();
}
void Stepp_motorClass::limit_ctrl()
{
	static int x0_old,y0_old;
	int x0 = LIMIT_X0;
	int y0 = LIMIT_Y0;
	int x1 = LIMIT_X1;
	int y1 = LIMIT_Y1;
	//setcurrentpositionを呼び出すとモータ止まる仕様らしいので，リミット離れたタイミングの1回のみ呼び出す
	if (!x0_old && x0)MOTOR_X.setCurrentPosition(0);
	if (!y0_old && y0)MOTOR_Y.setCurrentPosition(0);

	//1loopでの2度めの呼び出しは無視されるらしいので，手動のリミットは効かない
	if (!x0)
		if (MOTOR_X.distanceToGo()<0)MOTOR_X.stop();
	if (!x1)
		if (MOTOR_X.distanceToGo()>0)MOTOR_X.stop();
	if (!y0)
		if (MOTOR_Y.distanceToGo()<0)MOTOR_Y.stop();
	if (!y1)
		if (MOTOR_Y.distanceToGo()>0)MOTOR_Y.stop();

	x0_old = x0;
	y0_old = y0;

}
Stepp_motorClass Stepp_motor;


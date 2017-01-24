// 
// 
// 
#include "Stepp_motor.h"
#include "keypad.h"
AccelStepper MOTOR_X;
AccelStepper MOTOR_Y;
AccelStepper MOTOR_Z;

int SET_POS_MODE;//master_ctrl�Őݒ肵�Ă��郂�[�^(x,y,z)=(1,2,3)
String MOTOR_POS_SET;
int DRIVER_STATE;

void Stepp_motorClass::init()
{
	pinMode(PIN_ENABLE_XY, OUTPUT);
	pinMode(PIN_ENABLE_Z, OUTPUT);
	DRIVER_XY_OFF;
	DRIVER_Z_OFF;
	pinMode(PIN_SLIDE_SW, INPUT_PULLUP);//xy���[�^��Enable�𐧌䂷��
	pinMode(PIN_LIMIT_X0, INPUT_PULLUP);//���~�b�g�X�C�b�`
	pinMode(PIN_LIMIT_Y0, INPUT_PULLUP);//���~�b�g�X�C�b�`
	pinMode(PIN_LIMIT_X1, INPUT_PULLUP);//���~�b�g�X�C�b�`
	pinMode(PIN_LIMIT_Y1, INPUT_PULLUP);//���~�b�g�X�C�b�`

	pinMode(PIN_DIR_X, OUTPUT);	
	pinMode(PIN_DIR_Y, OUTPUT);
	pinMode(PIN_DIR_Z, OUTPUT);
	pinMode(PIN_STEP_X, OUTPUT);
	pinMode(PIN_STEP_Y, OUTPUT);
	pinMode(PIN_STEP_Z, OUTPUT);

	MOTOR_X = AccelStepper(AccelStepper::DRIVER, PIN_STEP_X, PIN_DIR_X);
	MOTOR_Y = AccelStepper(AccelStepper::DRIVER, PIN_STEP_Y, PIN_DIR_Y);
	MOTOR_Z = AccelStepper(AccelStepper::DRIVER, PIN_STEP_Z, PIN_DIR_Z);
	
	//runSpeed()�ŕ������]���鎞�Ɏg��
	speed_z = 1000;
	speed_x = 1000;
	speed_y = 1000;
	SET_POS_MODE = 0;//master_ctrl�Őݒ肵�Ă��郂�[�^x:1 y:2 z:3
	MOTOR_POS_SET = "";
	DRIVER_STATE = 0;//�h���C�o��L���ɂ��Ă邩�ǂ����Dz xy= 0:off off 1:on off 2:on on

	MOTOR_X.setMaxSpeed(speed_x);//[puls/sec] accelstepper�ł�1000�͒����Ȃ��݌v�����Ă���炵��(���̐�����)
	MOTOR_Y.setMaxSpeed(speed_y);//1000�ȏ�̐�������ƕςȓ��삷��Ƃ�����
	MOTOR_Z.setMaxSpeed(speed_z);
	MOTOR_X.setSpeed(speed_x);
	MOTOR_Y.setSpeed(speed_y);
	MOTOR_Z.setSpeed(speed_z);
	MOTOR_X.setAcceleration(1000);
	MOTOR_Y.setAcceleration(1000);
	MOTOR_Z.setAcceleration(1000);
}

void Stepp_motorClass::stepp_master_ctrl()
{
	X_master_input_ctrl();
	Y_master_input_ctrl();
	Z_master_input_ctrl();
	master_pad_controller_ctrl(500,1000);

	motors_run();
}
void Stepp_motorClass::master_pad_controller_ctrl(int xy,int z)
{
	if (SET_POS_MODE == 0)
	{
		//���Ƃ����move(0)�������Ȃ��̂Ń��~�b�g�����������ɏ����Ă���
		if (PAD_6 != 0 && LIMIT_X1)MOTOR_X.move(xy);
		if ((PAD_4 != 0 || PAD_5 != 0) && LIMIT_X0)MOTOR_X.move(-xy);//5�{�^����xy�������ʒu�Ɉړ�
		if (PAD_2 != 0 && LIMIT_Y1)MOTOR_Y.move(xy);
		if ((PAD_8 != 0 || PAD_5 != 0) && LIMIT_Y0)MOTOR_Y.move(-xy);
		if (PAD_3 != 0 )MOTOR_Z.move(z);
		if (PAD_1 != 0 )MOTOR_Z.move(-z);
	}
}
void Stepp_motorClass::X_master_input_ctrl()
{
	if (SET_POS_MODE != 1 && PRESSED_KEY == 'A')
		SET_POS_MODE = 1;
	else if (SET_POS_MODE == 1)
	{
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//�����̓���
		else if (PRESSED_KEY == '*')//+-�̓���
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-", "");
			else MOTOR_POS_SET = "-" + MOTOR_POS_SET;
		}
		else if (PRESSED_KEY == '#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length() - 1);//��������
		else if (PRESSED_KEY == 'A')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if (MOTOR_POS_SET.length() != 0)
					MOTOR_X.moveTo(MOTOR_POS_SET.toInt());//�󕶎�����Ȃ����
				else MOTOR_X.move(0);//�󕶎��Ȃ瓮���Ȃ�
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Y_master_input_ctrl()
{
	if (SET_POS_MODE != 2 && PRESSED_KEY == 'B')
		SET_POS_MODE = 2;
	else if (SET_POS_MODE == 2)
	{
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//�����̓���
		else if (PRESSED_KEY == '*')//+-�̓���
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-", "");
			else MOTOR_POS_SET = "-" + MOTOR_POS_SET;
		}
		else if (PRESSED_KEY == '#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length() - 1);//��������
		else if (PRESSED_KEY == 'B')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if (MOTOR_POS_SET.length() != 0)
					MOTOR_Y.moveTo(MOTOR_POS_SET.toInt());//�󕶎�����Ȃ����
				else MOTOR_Y.move(0);//�󕶎��Ȃ瓮���Ȃ�
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::Z_master_input_ctrl()
{
	if (SET_POS_MODE != 3 && PRESSED_KEY=='C')
		SET_POS_MODE = 3;
	else if (SET_POS_MODE == 3)
	{	
		if (PRESSED_KEY >= '0'&& PRESSED_KEY <= '9')
			MOTOR_POS_SET += PRESSED_KEY;//�����̓���
		else if (PRESSED_KEY == '*')//+-�̓���
		{
			if (MOTOR_POS_SET.startsWith("-"))
				MOTOR_POS_SET.replace("-","");
			else MOTOR_POS_SET= "-"+MOTOR_POS_SET;
		}
		else if (PRESSED_KEY=='#' && MOTOR_POS_SET.length() > 0)
			MOTOR_POS_SET.remove(MOTOR_POS_SET.length()-1);//��������
		else if (PRESSED_KEY=='C')
		{
			if (isFloat(MOTOR_POS_SET))
			{
				if(MOTOR_POS_SET.length() != 0)
					MOTOR_Z.moveTo(MOTOR_POS_SET.toInt());//�󕶎�����Ȃ����
				else MOTOR_Z.move(0);//�󕶎��Ȃ瓮���Ȃ�
			}
			SET_POS_MODE = 0;
			MOTOR_POS_SET = "";
		}
	}
}
void Stepp_motorClass::stepp_slave_ctrl(String command)
{
	static String command_old = "";

	if (!command.equals(command_old))//�R�}���h�������ꍇ�̓X�L�b�v(������Ȃ��ƃN�\�d������)
	{
		String command_trim = command;
		command_trim.replace(" ", "");//�X�y�[�X�Ȃ���
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
		if (isFloat(val)) MOTOR_X.moveTo(val.toInt());//�ڕW�ʒu�̎w��DMOTOR_X.run()�œ���
	}
	if (command.equals("STOP_X") || command.equals("STOP"))MOTOR_X.stop();//�~�܂�ʒu�ɖڕW���W��ύX


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
boolean Stepp_motorClass::isFloat(String tString) {//String�𐔒l���o���邩�ǂ���
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
	MOTOR_X.move(0);//���̏�Ŏ~�܂�
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
	//setcurrentposition���Ăяo���ƃ��[�^�~�܂�d�l�炵���̂ŁC���~�b�g���ꂽ�^�C�~���O��1��̂݌Ăяo��
	if (!x0_old && x0)MOTOR_X.setCurrentPosition(0);
	if (!y0_old && y0)MOTOR_Y.setCurrentPosition(0);

	//1loop�ł�2�x�߂̌Ăяo���͖��������炵���̂ŁC�蓮�̃��~�b�g�͌����Ȃ�
	if (!x0)
		if (MOTOR_X.distanceToGo()<0)MOTOR_X.move(0);
	if (!x1)
		if (MOTOR_X.distanceToGo()>0)MOTOR_X.move(0);
	if (!y0)
		if (MOTOR_Y.distanceToGo()<0)MOTOR_Y.move(0);
	if (!y1)
		if (MOTOR_Y.distanceToGo()>0)MOTOR_Y.move(0);

	x0_old = x0;
	y0_old = y0;

}
Stepp_motorClass Stepp_motor;


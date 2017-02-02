#include "keypad.h"//�L�[�p�b�h�D���C�u�����g���Ɗ��荞�݂��ʓ|�������̂Ŏ���
#include "OLED.h"
#include "Stepp_motor.h"

#include <FlexiTimer2.h>//�^�C�}���荞�݁iTimer2���g���̂�9,10����PWM�o���Ȃ��Ȃ�j
#define TIMER2_INTERVAL (float)0.2//[ms](1000pulse/sec�ȏ�̊��荞�݂��]�܂�������)
void timer_interrupt() {//���C���̑���n�̏���������

	Keypad.get_status();//pad��ԍX�V	
	mode_update();//CTRL_MODE�X�V
	//stepper
	if (CTRL_MODE == MODE_MASTER)Stepp_motor.master_ctrl();
	if (CTRL_MODE == MODE_SLAVE)Stepp_motor.slave_ctrl();
	AD4 = analogRead(4);
}
void mode_update()
{
	if (PAD_D == PRESSED && SET_POS_MODE == NONE)CTRL_MODE++;
	if (CTRL_MODE > MODE_SLAVE)CTRL_MODE = MODE_SLEEP;
	
	if (CTRL_MODE == MODE_SLEEP) 
	{
		DRIVER_Z_OFF;
		DRIVER_XY_OFF; 
		DRIVER_STATE = STATE_OFF_OFF;
	}
	else
	{
		DRIVER_Z_ON;
		if (SLIDE_SW)
		{
			DRIVER_XY_OFF;
			DRIVER_STATE = STATE_OFF_ON;

		}
		else
		{
			DRIVER_XY_ON;
			DRIVER_STATE = STATE_ON_ON;
		}
	}

	Stepp_motor.mode_changed_ctrl();//���[�h���؂�ւ�����^�C�~���O�ōs����������
}


void setup()
{
	Keypad.init();
	OLED.init();
	Stepp_motor.init();
	
	// Set the Prescaler to 16 (16000KHz/16 = 1MHz)
	// WARNING: Above 200KHz 10-bit results are not reliable.
	//ADCSRA |= B00000100;
	cbi(ADCSRA, ADPS2);//8�����ɂ��Ă݂�
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
	// Without this, the internal interrupt will not trigger.
	//ADCSRA |= B00001000;
	//sbi(ADCSRA, ADIE);

	FlexiTimer2::set(TIMER2_INTERVAL, timer_interrupt);
	FlexiTimer2::start();

	Serial.begin(9600);
}

void loop()//�f�o�b�O�n�̏�������������(�d������)
{
	u8g.firstPage();
	do {
		
		//PC�f�o�b�O(�ǂ����V���A����M�ɉe��������ۂ����炠��܂��Ȃ��ق�����������)
		//Keypad.serial_debug();
		char buff[SLAVE_COMMAND_LENGTH];
		
		mode_view();//MODE�\��			
		pos_view();//pos�\��	
		driver_state_view();//���^�h���쓮���Ă邩�ǂ���

		//AD�\��
		sprintf(buff, "%d", AD4);
		u8g.drawStr(2, 50, buff);

		//COMMAND
		str_to_char_array(SLAVE_COMMAND, buff, SLAVE_COMMAND_LENGTH);
		u8g.drawStr(40, 50, buff);

		limit_view();//LIMIT


	} while (u8g.nextPage());

}
void limit_view()
{
	String str = "";
	if (!LIMIT_X0)str += "X- ";
	if (!LIMIT_X1)str += "X+ ";
	if (!LIMIT_Y0)str += "Y- ";
	if (!LIMIT_Y1)str += "Y+ ";

	if (str.length() != 0) 
	{
		char buff[30] = { '/0' };
		str = "LIMIT:" + str;
		str_to_char_array(str, buff, 30);
		u8g.drawStr(2, 60, buff);

		analogWrite(PIN_BUZZER, 50);//�u�U�[ tone�֐����g����duration�Ԃ�J��Ԃ���Đ��䐫�Ȃ��Ȃ�
	}
	else analogWrite(PIN_BUZZER, 0);
}
void driver_state_view()
{//�쓮���̃��[�^��*��\��
	switch (DRIVER_STATE)
	{
		case STATE_OFF_OFF:break;
		case STATE_OFF_ON:
			u8g.drawStr(115, 40, "*");//z�쓮��
			break;
		case STATE_ON_ON:
			u8g.drawStr(115, 20, "*");//x�쓮��
			u8g.drawStr(115, 30, "*");//y�쓮��
			u8g.drawStr(115, 40, "*");//z�쓮��
			break;
		default:break;
	}
}
void mode_view()
{
	if (CTRL_MODE == MODE_SLEEP)u8g.drawStr(45, 10, "SLEEP");
	else if (CTRL_MODE == MODE_MASTER)u8g.drawStr(45, 10, "MASTER");
	else if (CTRL_MODE == MODE_SLAVE)u8g.drawStr(45, 10, "SLAVE");
}
void pos_view()
{
	char buff[7];
	double pos_x = PULSE_TO_mm(MOTOR_X.currentPosition());
	double pos_y = PULSE_TO_mm(MOTOR_Y.currentPosition());
	double pos_z = PULSE_TO_DEG(MOTOR_Z.currentPosition());

	//�P�ʕ\��
	u8g.drawStr(90, 20, "mm");//x
	u8g.drawStr(90, 30, "mm");//y
	u8g.drawStr(85, 40, "deg");//z

	if (SET_POS_MODE == 0)
	{
		//sprintf(buff, "%ld", MOTOR_X.currentPosition());
		dtostrf(pos_x, 7, 2, buff);
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buff);
		//sprintf(buff, "%ld", MOTOR_Y.currentPosition());
		dtostrf(pos_y, 7, 2, buff);
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buff);
		//sprintf(buff, "%1f", pos_z);
		dtostrf(pos_z, 7, 2, buff);
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buff);
	}
	else if (SET_POS_MODE == MODE_X)
	{
		u8g.drawStr(2, 20, "X->");
		str_to_char_array(MOTOR_POS_SET, buff, 7);
		u8g.drawStr(25, 20, buff);
		u8g.drawStr(25, 20, "_______");

		dtostrf(pos_y, 7, 2, buff);
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buff);
		dtostrf(pos_z, 7, 2, buff);
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buff);
	}
	else if (SET_POS_MODE == MODE_Y)
	{
		dtostrf(pos_x, 7, 2, buff);
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buff);

		u8g.drawStr(2, 30, "Y->");
		str_to_char_array(MOTOR_POS_SET, buff, 7);
		u8g.drawStr(25, 30, buff);
		u8g.drawStr(25, 30, "_______");

		dtostrf(pos_z, 7, 2, buff);
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buff);
	}
	else if (SET_POS_MODE == MODE_Z)
	{
		dtostrf(pos_x, 7, 2, buff);
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buff);
		dtostrf(pos_y, 7, 2, buff);
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buff);

		u8g.drawStr(2, 40, "Z->");
		str_to_char_array(MOTOR_POS_SET, buff, 6);
		u8g.drawStr(25, 40, buff);
		u8g.drawStr(25, 40, "_______");
	}
}
void str_to_char_array(String str, char *s,int array_length)
{
	int i = 0;
	for (i = 0; i < array_length; i++)s[i] = ' ';//�L�����z��̃N���A
	s[array_length] = '\0';//������ԂŖ��[�����ȊO�̂��̂������Ă���ꍇ���������̂Ŏ蓮�Ŋi�[
	for (i = 0; i < array_length; i++)
		if (str[i] != '\0')s[i] = str[i]; else break;
}
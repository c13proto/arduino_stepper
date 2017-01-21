#include "keypad.h"//�L�[�p�b�h�D���C�u�����g���Ɗ��荞�݂��ʓ|�������̂Ŏ���
#include "OLED.h"
#include "Stepp_motor.h"

#include <FlexiTimer2.h>//�^�C�}���荞�݁iTimer2���g���̂�9,10����PWM�o���Ȃ��Ȃ�j
#define TIMER2_INTERVAL (float)1//[ms](������ׂ������Ă��ς��Ȃ�����)
#define MODE_SLEEP 0
#define MODE_MASTER 1
#define MODE_SLAVE 2
int CTRL_MODE = MODE_SLEEP;
int AD8;//analogRead(8);
void timer_interrupt() {//���C���̑���n�̏���������

	Keypad.get_status();//pad��ԍX�V	
	mode_update();//CTRL_MODE�X�V
	AD8 = analogRead(8);
	//stepper
	if (CTRL_MODE == MODE_MASTER)master_ctrl();
	if (CTRL_MODE == MODE_SLAVE)slave_ctrl();


}
void mode_update()
{
	static char mode_old;
	if (PAD_x == 1)CTRL_MODE++;
	if (CTRL_MODE > 2)CTRL_MODE = MODE_SLEEP;

	if (CTRL_MODE == MODE_SLEEP)DRIVER_OFF;
	else DRIVER_ON;

	if(mode_old!= CTRL_MODE)Stepp_motor.stepp_stop();//���[�h���؂�ւ��^�C�~���O�̓��[�^�̃|�W�V�����ێ����Ȃ�
	mode_old = CTRL_MODE;
}
//�}�X�^�[���[�h�̏���
void master_ctrl()
{
	Stepp_motor.stepp_master_ctrl();
}

//�X���[�u���[�h�̏���
const int COMMAND_LENGTH = 10;//�R�}���h�̍ő啶����
char buff[COMMAND_LENGTH] = {};//
String COMMAND = "ready(^^)";
int char_counter = 0;
void slave_ctrl()
{
	command_update();//�R�}���h��M
	Stepp_motor.stepp_slave_ctrl(COMMAND);

}
void command_update()
{
	if (Serial.available())
	{
		char data = Serial.read();

		//10����+e�ł̔��f
		if (data != ';') {//';'�𑗂��邱�ƂŃR�}���h�ƔF��������
			buff[char_counter] = data;
			char_counter++;
		}
		else {
			int i = 0;
			if (char_counter == COMMAND_LENGTH)
			{
				COMMAND = "          ";//COMMAND_LENGTH�Ԃ�̃X�y�[�X�p�ӂ��Ă��Ȃ��Ɗi�[���Ă���Ȃ��݂���
				for (i = 0; i < COMMAND_LENGTH; i++)COMMAND[i] = buff[i];
			}
			else COMMAND = "no command";
			char_counter = 0;
		}
	}
}
void setup()
{
	Keypad.init();
	OLED.init();
	Stepp_motor.init();

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

		//MODE�\��
		if(CTRL_MODE==0)u8g.drawStr(45, 10, "SLEEP");
		else if(CTRL_MODE == 1)u8g.drawStr(45, 10, "MASTER");
		else if (CTRL_MODE == 2)u8g.drawStr(45, 10, "SLAVE");
		
		//pos�\��
		char buf[COMMAND_LENGTH];
		sprintf(buf, "%ld", motorX.currentPosition());
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buf);
		sprintf(buf, "%ld", motorY.currentPosition());
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buf);
		sprintf(buf, "%ld", motorZ.currentPosition());
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buf);
		
		//AD�\��
		sprintf(buf, "%d", AD8);
		u8g.drawStr(2, 50, buf);

		//COMMAND
		int i = 0;
		for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = COMMAND[i];
		u8g.drawStr(40, 50, buf);

	} while (u8g.nextPage());


}

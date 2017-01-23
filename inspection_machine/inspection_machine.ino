#include "keypad.h"//キーパッド．ライブラリ使うと割り込みが面倒だったので自作
#include "OLED.h"
#include "Stepp_motor.h"

#include <FlexiTimer2.h>//タイマ割り込み（Timer2を使うので9,10からPWM出せなくなる）
#define TIMER2_INTERVAL (float)1//[ms](これより細かくしても変わらないかも)
#define MODE_SLEEP 0
#define MODE_MASTER 1
#define MODE_SLAVE 2
int CTRL_MODE = MODE_SLEEP;
int AD8;//analogRead(8);
void timer_interrupt() {//メインの操作系の処理こっち

	Keypad.get_status();//pad状態更新	
	mode_update();//CTRL_MODE更新
	AD8 = analogRead(8);
	//stepper
	if (CTRL_MODE == MODE_MASTER)master_ctrl();
	if (CTRL_MODE == MODE_SLAVE)slave_ctrl();


}
void mode_update()
{
	static char mode_old;
	if (PAD_D == 1)CTRL_MODE++;
	if (CTRL_MODE > 2)CTRL_MODE = MODE_SLEEP;

	if (CTRL_MODE == MODE_SLEEP)DRIVER_OFF;
	else DRIVER_ON;

	if (mode_old != CTRL_MODE)
	{
		Stepp_motor.motors_stop();//モードが切り替わるタイミングはモータのポジション保持しない
		SET_POS_MODE = 0;
	}
	mode_old = CTRL_MODE;
}
//マスターモードの処理
void master_ctrl()
{
	Stepp_motor.stepp_master_ctrl();
}

//スレーブモードの処理
const int COMMAND_LENGTH = 10;//コマンドの最大文字数
char command_buff[COMMAND_LENGTH] = {};//
String COMMAND = "no command";
int char_counter = 0;
void slave_ctrl()
{
	command_update();//コマンド受信
	Stepp_motor.stepp_slave_ctrl(COMMAND);

}
void command_update()
{
	if (Serial.available())
	{
		char data = Serial.read();

		//10文字+eでの判断
		if (data != ';') {//';'を送られることでコマンドと認識させる
			command_buff[char_counter] = data;
			char_counter++;
		}
		else {
			int i = 0;
			if (char_counter == COMMAND_LENGTH)
			{
				COMMAND = "          ";//COMMAND_LENGTHぶんのスペース用意してやらないと格納してくれないみたい
				for (i = 0; i < COMMAND_LENGTH; i++)COMMAND[i] = command_buff[i];
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

void loop()//デバッグ系の処理をこっちに(重いから)
{
	u8g.firstPage();
	do {
		
		//PCデバッグ(どうやらシリアル受信に影響するっぽいからあんまやらないほうがいいかも)
		//Keypad.serial_debug();
		//if (PRESSED_KEY != "")Serial.print(PRESSED_KEY);
		
		//MODE表示
		if(CTRL_MODE==0)u8g.drawStr(45, 10, "SLEEP");
		else if(CTRL_MODE == 1)u8g.drawStr(45, 10, "MASTER");
		else if (CTRL_MODE == 2)u8g.drawStr(45, 10, "SLAVE");
		
		//pos表示
		char buf[COMMAND_LENGTH];
		if (SET_POS_MODE == 0)
		{
			sprintf(buf, "%ld", MOTOR_X.currentPosition());
			u8g.drawStr(2, 20, "X:");
			u8g.drawStr(17, 20, buf);
			sprintf(buf, "%ld", MOTOR_Y.currentPosition());
			u8g.drawStr(2, 30, "Y:");
			u8g.drawStr(17, 30, buf);
			sprintf(buf, "%ld", MOTOR_Z.currentPosition());
			u8g.drawStr(2, 40, "Z:");
			u8g.drawStr(17, 40, buf);
		}
		else if (SET_POS_MODE == 1)
		{
			int i = 0;
			u8g.drawStr(2, 20, "X->");
			for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = ' ';
			for (i = 0; i < MOTOR_POS_SET.length(); i++)buf[i] = MOTOR_POS_SET[i];
			u8g.drawStr(25, 20, buf);
			u8g.drawStr(25, 20, "__________");

			sprintf(buf, "%ld", MOTOR_Y.currentPosition());
			u8g.drawStr(2, 30, "Y:");
			u8g.drawStr(17, 30, buf);
			sprintf(buf, "%ld", MOTOR_Z.currentPosition());
			u8g.drawStr(2, 40, "Z:");
			u8g.drawStr(17, 40, buf);
		}
		else if (SET_POS_MODE == 2)
		{
			sprintf(buf, "%ld", MOTOR_X.currentPosition());
			u8g.drawStr(2, 20, "X:");
			u8g.drawStr(17, 20, buf);

			int i = 0;
			u8g.drawStr(2, 30, "Y->");
			for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = ' ';
			for (i = 0; i < MOTOR_POS_SET.length(); i++)buf[i] = MOTOR_POS_SET[i];
			u8g.drawStr(25, 30, buf);
			u8g.drawStr(25, 30, "__________");

			sprintf(buf, "%ld", MOTOR_Z.currentPosition());
			u8g.drawStr(2, 40, "Z:");
			u8g.drawStr(17, 40, buf);
		}
		else if (SET_POS_MODE == 3)
		{
			sprintf(buf, "%ld", MOTOR_X.currentPosition());
			u8g.drawStr(2, 20, "X:");
			u8g.drawStr(17, 20, buf);
			sprintf(buf, "%ld", MOTOR_Y.currentPosition());
			u8g.drawStr(2, 30, "Y:");
			u8g.drawStr(17, 30, buf);

			int i = 0;
			u8g.drawStr(2, 40, "Z->");
			for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = ' ';
			for (i = 0; i < MOTOR_POS_SET.length(); i++)buf[i] = MOTOR_POS_SET[i];
			u8g.drawStr(25, 40, buf);
			u8g.drawStr(25, 40, "__________");
		}
		
		//AD表示
		sprintf(buf, "%d", AD8);
		u8g.drawStr(2, 50, buf);

		//COMMAND
		int i = 0;
		for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = COMMAND[i];
		u8g.drawStr(40, 50, buf);

	} while (u8g.nextPage());


}

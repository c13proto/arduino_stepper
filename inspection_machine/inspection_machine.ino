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
	if (PAD_x == 1)CTRL_MODE++;
	if (CTRL_MODE > 2)CTRL_MODE = MODE_SLEEP;

	if (CTRL_MODE == MODE_SLEEP)DRIVER_OFF;
	else DRIVER_ON;

	if(mode_old!= CTRL_MODE)Stepp_motor.stepp_stop();//モードが切り替わるタイミングはモータのポジション保持しない
	mode_old = CTRL_MODE;
}
//マスターモードの処理
void master_ctrl()
{
	Stepp_motor.stepp_master_ctrl();
}

//スレーブモードの処理
const int COMMAND_LENGTH = 10;//コマンドの最大文字数
char buff[COMMAND_LENGTH] = {};//
String COMMAND = "ready(^^)";
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
			buff[char_counter] = data;
			char_counter++;
		}
		else {
			int i = 0;
			if (char_counter == COMMAND_LENGTH)
			{
				COMMAND = "          ";//COMMAND_LENGTHぶんのスペース用意してやらないと格納してくれないみたい
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

void loop()//デバッグ系の処理をこっちに(重いから)
{
	u8g.firstPage();
	do {
		
		//PCデバッグ(どうやらシリアル受信に影響するっぽいからあんまやらないほうがいいかも)
		//Keypad.serial_debug();

		//MODE表示
		if(CTRL_MODE==0)u8g.drawStr(45, 10, "SLEEP");
		else if(CTRL_MODE == 1)u8g.drawStr(45, 10, "MASTER");
		else if (CTRL_MODE == 2)u8g.drawStr(45, 10, "SLAVE");
		
		//pos表示
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
		
		//AD表示
		sprintf(buf, "%d", AD8);
		u8g.drawStr(2, 50, buf);

		//COMMAND
		int i = 0;
		for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = COMMAND[i];
		u8g.drawStr(40, 50, buf);

	} while (u8g.nextPage());


}

#include "keypad.h"//キーパッド．ライブラリ使うと割り込みが面倒だったので自作
#include "OLED.h"
#include "Stepp_motor.h"

#include <FlexiTimer2.h>//タイマ割り込み（Timer2を使うので9,10からPWM出せなくなる）
#define TIMER2_INTERVAL (float)1//[ms](これより細かくしても変わらないかも)
#define MODE_SLEEP 0
#define MODE_MASTER 1
#define MODE_SLAVE 2
int CTRL_MODE = MODE_SLEEP;
void timer_interrupt() {//メインの操作系の処理こっち

	Keypad.get_status();//pad状態更新	
	mode_update();//CTRL_MODE更新
	//stepper
	if (CTRL_MODE == MODE_MASTER)master_ctrl();
	if (CTRL_MODE == MODE_SLAVE)slave_ctrl();


}
void mode_update()
{
	if (PAD_x == 1)CTRL_MODE++;
	if (CTRL_MODE > 2)CTRL_MODE = MODE_SLEEP;

	if (CTRL_MODE == MODE_SLEEP)DRIVER_OFF;
	else DRIVER_ON;
}
//マスターモードの処理
void master_ctrl()
{
	if (PAD_0) {
		// If at the end of travel go to the other end
		if (motorZ.distanceToGo() == 0)
			motorZ.moveTo(-motorZ.currentPosition());
		motorZ.run();
	}
}
//スレーブモードの処理
const int COMMAND_LENGTH = 10;//コマンドの文字数
char buff[COMMAND_LENGTH] = {};//
String COMMAND = "ready!";
int char_counter = 0;
void slave_ctrl()
{
	command_update();//コマンド受信

}
void command_update()
{
	if (Serial.available())
	{
		char data = Serial.read();
		if (data != 'e') {//COMMAND_LENGTH+1文字目にeを送られることでコマンドと認識させる
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
			else COMMAND = "error";
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
	int pos = motorZ.currentPosition();

	u8g.firstPage();
	do {
		//PCデバッグ(これめちゃ重)
		Serial.println(PAD_D);
		//MODE表示
		if(CTRL_MODE==0)u8g.drawStr(45, 10, "SLEEP");
		else if(CTRL_MODE == 1)u8g.drawStr(45, 10, "MASTER");
		else if (CTRL_MODE == 2)u8g.drawStr(45, 10, "SLAVE");
		
		//pos表示
		char buf[COMMAND_LENGTH];
		sprintf(buf, "%d", pos);
		u8g.drawStr(4, 20, buf);
		
		//AD表示
		sprintf(buf, "%d", analogRead(A8));
		u8g.drawStr(4, 30, buf);

		//COMMAND
		int i = 0;
		for (i = 0; i < COMMAND_LENGTH; i++)buf[i] = COMMAND[i];
		u8g.drawStr(4, 40, buf);

	} while (u8g.nextPage());


}

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


}
void mode_update()
{
	if (PAD_x == 1)CTRL_MODE++;
	if (CTRL_MODE > 2)CTRL_MODE = MODE_SLEEP;

	if (CTRL_MODE == MODE_SLEEP)DRIVER_OFF;
	else DRIVER_ON;
}
void master_ctrl()
{
	if (PAD_0) {
		// If at the end of travel go to the other end
		if (motorZ.distanceToGo() == 0)
			motorZ.moveTo(-motorZ.currentPosition());
		motorZ.run();
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
		Serial.println(PAD_s);


		//MODE表示
		if(CTRL_MODE==0)u8g.drawStr(45, 10, "SLEEP");
		else if(CTRL_MODE == 1)u8g.drawStr(45, 10, "MASTER");
		else if (CTRL_MODE == 2)u8g.drawStr(45, 10, "SLAVE");
		
		//pos表示
		char buf[10];
		sprintf(buf, "%d", pos);
		u8g.drawStr(4, 20, buf);
		//AD表示
		sprintf(buf, "%d", analogRead(A8));
		u8g.drawStr(4, 30, buf);

	} while (u8g.nextPage());


}

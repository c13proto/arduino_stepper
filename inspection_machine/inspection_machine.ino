#include "keypad.h"//キーパッド．ライブラリ使うと割り込みが面倒だったので自作
#include "OLED.h"
#include "Stepp_motor.h"

#include <FlexiTimer2.h>//タイマ割り込み（Timer2を使うので9,10からPWM出せなくなる）
int AD4;//analogRead(4);
int CTRL_MODE = 0;
#define MODE_SLEEP 0
#define MODE_MASTER 1
#define MODE_SLAVE 2
#define TIMER2_INTERVAL (float)0.2//[ms](1000pulse/sec以上の割り込みが望ましいかな)
void timer_interrupt() {//メインの操作系の処理こっち

	Keypad.get_status();//pad状態更新	
	mode_update();//CTRL_MODE更新
	//stepper
	if (CTRL_MODE == MODE_MASTER)master_ctrl();
	if (CTRL_MODE == MODE_SLAVE)slave_ctrl();
	AD4 = analogRead(4);
}
void mode_update()
{
	static char mode_old;
	if (PAD_D == PRESSED)CTRL_MODE++;
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
	
	// Set the Prescaler to 16 (16000KHz/16 = 1MHz)
	// WARNING: Above 200KHz 10-bit results are not reliable.
	//ADCSRA |= B00000100;
	cbi(ADCSRA, ADPS2);//8分周にしてみる
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

void loop()//デバッグ系の処理をこっちに(重いから)
{
	u8g.firstPage();
	do {
		
		//PCデバッグ(どうやらシリアル受信に影響するっぽいからあんまやらないほうがいいかも)
		//Keypad.serial_debug();
		char buf[COMMAND_LENGTH];
		
		mode_view();//MODE表示			
		pos_view();//pos表示	
		driver_state_view();//モタドラ駆動してるかどうか

		//AD表示
		sprintf(buf, "%d", AD4);
		u8g.drawStr(2, 50, buf);

		//COMMAND
		str_to_char_array(COMMAND, buf, COMMAND_LENGTH);
		u8g.drawStr(40, 50, buf);

	} while (u8g.nextPage());

}
void driver_state_view()
{//駆動中のモータに*を表示
	switch (DRIVER_STATE)
	{
		case STATE_OFF_OFF:break;
		case STATE_OFF_ON:
			u8g.drawStr(110, 40, "*");//z駆動中
			break;
		case STATE_ON_ON:
			u8g.drawStr(110, 20, "*");//x駆動中
			u8g.drawStr(110, 30, "*");//y駆動中
			u8g.drawStr(110, 40, "*");//z駆動中
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
	else if (SET_POS_MODE == MODE_X)
	{
		u8g.drawStr(2, 20, "X->");
		str_to_char_array(MOTOR_POS_SET, buf, COMMAND_LENGTH);
		u8g.drawStr(25, 20, buf);
		u8g.drawStr(25, 20, "__________");

		sprintf(buf, "%ld", MOTOR_Y.currentPosition());
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buf);
		sprintf(buf, "%ld", MOTOR_Z.currentPosition());
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buf);
	}
	else if (SET_POS_MODE == MODE_Y)
	{
		sprintf(buf, "%ld", MOTOR_X.currentPosition());
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buf);

		u8g.drawStr(2, 30, "Y->");
		str_to_char_array(MOTOR_POS_SET, buf, COMMAND_LENGTH);
		u8g.drawStr(25, 30, buf);
		u8g.drawStr(25, 30, "__________");

		sprintf(buf, "%ld", MOTOR_Z.currentPosition());
		u8g.drawStr(2, 40, "Z:");
		u8g.drawStr(17, 40, buf);
	}
	else if (SET_POS_MODE == MODE_Z)
	{
		sprintf(buf, "%ld", MOTOR_X.currentPosition());
		u8g.drawStr(2, 20, "X:");
		u8g.drawStr(17, 20, buf);
		sprintf(buf, "%ld", MOTOR_Y.currentPosition());
		u8g.drawStr(2, 30, "Y:");
		u8g.drawStr(17, 30, buf);

		u8g.drawStr(2, 40, "Z->");
		str_to_char_array(MOTOR_POS_SET, buf, COMMAND_LENGTH);
		u8g.drawStr(25, 40, buf);
		u8g.drawStr(25, 40, "__________");
	}
}
void str_to_char_array(String str, char *s,int array_length)
{
	int i = 0;
	for (i = 0; i < array_length; i++)s[i] = ' ';//キャラ配列のクリア
	for (i = 0; i < array_length; i++)
		if (str[i] != '\0')s[i] = str[i]; else break;
}
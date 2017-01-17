
#include <Keypad.h>//�L�[�p�b�h�̐ݒ�
KeyState PAD_1; KeyState PAD_2; KeyState PAD_3; KeyState PAD_A;
KeyState PAD_4; KeyState PAD_5; KeyState PAD_6; KeyState PAD_B;
KeyState PAD_7; KeyState PAD_8; KeyState PAD_9; KeyState PAD_C;
KeyState PAD_x; KeyState PAD_0; KeyState PAD_s; KeyState PAD_D;
char keys[4][4] =
{
	{ '1','2','3','A' },
	{ '4','5','6','B' },
	{ '7','8','9','C' },
	{ '*','0','#','D' }
};
byte rowPins[4] = { 29, 28, 27, 26 };
byte colPins[4] = { 25, 24, 23,22 };
Keypad KEY = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
void keypadEvent(KeypadEvent key) {
	KeyState state = KEY.getState();
	switch (key) {
	case '1': PAD_1 = state; break;
	case '2': PAD_2 = state; break;
	case '3': PAD_3 = state; break;
	case '4': PAD_4 = state; break;
	case '5': PAD_5 = state; break;
	case '6': PAD_6 = state; break;
	case '7': PAD_7 = state; break;
	case '8': PAD_8 = state; break;
	case '9': PAD_9 = state; break;
	case '0': PAD_0 = state; break;
	case 'A': PAD_A = state; break;
	case 'B': PAD_B = state; break;
	case 'C': PAD_C = state; break;
	case 'D': PAD_D = state; break;
	case '*': PAD_x = state; break;
	case '#': PAD_s = state; break;
	}
}

#include <U8glib.h>//OCED�̐ݒ�(�����������Ȃ�̂ł�߂�)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);	// Fast I2C / TWI 

#include <MultiStepper.h>
#include <AccelStepper.h>//�X�e�b�s���O���[�^�ݒ�
#define LIMIT_Z 11
#define LIMIT_Y 10
#define LIMIT_X 9
#define DIR_Z 7
#define DIR_Y 6
#define DIR_X 5
#define STEP_Z 4
#define STEP_Y 3
#define STEP_X 2
#define MOTOR_ENABLE 8//
AccelStepper motorZ(AccelStepper::DRIVER, STEP_Z, DIR_Z);
//AccelStepper motorX(AccelStepper::DRIVER, STEP_X, DIR_X);
//AccelStepper motorY(AccelStepper::DRIVER, STEP_Y, DIR_Y);
void stepper_init()
{
	pinMode(MOTOR_ENABLE,OUTPUT);
	digitalWrite(MOTOR_ENABLE, HIGH);//�����LOW�ɂ��Ȃ��ƃ��^�h���쓮���Ȃ�

	pinMode(DIR_Z, OUTPUT);
	pinMode(STEP_Z, OUTPUT);
	pinMode(DIR_X, OUTPUT);
	pinMode(STEP_X, OUTPUT);
	motorZ.setMaxSpeed((float)10000);
	motorZ.setSpeed((float)10000);
	motorZ.setAcceleration((float)1000);
	motorZ.moveTo(200*32);

	digitalWrite(MOTOR_ENABLE, LOW);//�����LOW�ɂ��Ȃ��ƃ��^�h���쓮���Ȃ�
}


#include <FlexiTimer2.h>
/* �^�C�}���荞�݊Ԋu */
#define TIMER2_INTERVAL (float)0.5//[ms]
/* �^�C�}���荞�� */
void timer_interrupt() {//���C���̑���n�̏���������
	/* �������� */
	//ad�ϊ�
	int ad = analogRead(A8);

	//stepper
	// If at the end of travel go to the other end
	if (motorZ.distanceToGo() == 0)
		motorZ.moveTo(-motorZ.currentPosition());
	motorZ.run();
}

void setup()
{
  /* add setup code here */
	KEY.addEventListener(keypadEvent); //add an event listener for this keypad
	u8g.setFont(u8g_font_8x13r);//oled�t�H���g�̎w��
	stepper_init();

	FlexiTimer2::set(TIMER2_INTERVAL, timer_interrupt);
	FlexiTimer2::start();

	Serial.begin(9600);

}

void loop()//�f�o�b�O�n�̏�������������(�d������)
{
	int pos = motorZ.currentPosition();

	u8g.firstPage();
	do {


		//PC�f�o�b�O(����߂���d)
		Serial.println(pos);

		char buf[10];
		sprintf(buf, "%d", pos);
		u8g.drawStr(4, 10, buf);
		sprintf(buf, "%d", analogRead(A8));
		u8g.drawStr(4, 20, buf);

	} while (u8g.nextPage());


}

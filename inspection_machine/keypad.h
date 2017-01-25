// keypad.h

#ifndef _KEYPAD_h
#define _KEYPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define cbi(PIN,bit)     PIN &= ~(1<<bit)//digitalWrite(PIN,0)の代わり．PINのbit番目をLowに
#define sbi(PIN,bit)     PIN |=  (1<<bit)//digitalWrite(PIN,1)の代わり．PINのbit番目をHighに
#define ibi(PIN,bit)	bit_is_set(PIN, bit)//digitalRead(PIN)の代わり．PINのbitがLOWなら0を，Highなら1を返す
extern int PAD_1; extern int PAD_2; extern int PAD_3; extern int PAD_A;
extern int PAD_4; extern int PAD_5; extern int PAD_6; extern int PAD_B;
extern int PAD_7; extern int PAD_8; extern int PAD_9; extern int PAD_C;
extern int PAD_x; extern int PAD_0; extern int PAD_s; extern int PAD_D;
#define NONE	 0
#define PRESSED  1
#define HOLD	 2
#define RELEASED 3
extern char PRESSED_KEY;
class KeypadClass
{
 protected:
	 char get_pressed_key();
	 void input_pad_state(int *PAD, int pad);

 public:
	void init();
	void get_status();
	void serial_debug();
	
};

extern KeypadClass Keypad;

#endif


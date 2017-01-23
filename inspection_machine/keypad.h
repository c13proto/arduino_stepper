// keypad.h

#ifndef _KEYPAD_h
#define _KEYPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class KeypadClass
{
 protected:
	 char get_pressed_key();

 public:
	void init();
	void get_status();
	void serial_debug();
	
};

extern int PAD_1;extern int PAD_2;extern int PAD_3;extern int PAD_A;
extern int PAD_4;extern int PAD_5;extern int PAD_6;extern int PAD_B;
extern int PAD_7;extern int PAD_8;extern int PAD_9;extern int PAD_C;
extern int PAD_x;extern int PAD_0;extern int PAD_s;extern int PAD_D;

extern char PRESSED_KEY;

extern KeypadClass Keypad;

#endif


// 
// 
// 

#include "keypad.h"

int PAD_1;int PAD_2; int PAD_3;int PAD_A;
int PAD_4;int PAD_5; int PAD_6;int PAD_B;
int PAD_7;int PAD_8; int PAD_9;int PAD_C;
int PAD_x;int PAD_0; int PAD_s;int PAD_D;



void KeypadClass::init()
{
	pinMode(29, INPUT_PULLUP);
	pinMode(28, INPUT_PULLUP);
	pinMode(27, INPUT_PULLUP);
	pinMode(26, INPUT_PULLUP);

	pinMode(25, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(22, OUTPUT);
}

void KeypadClass::get_status()
{

	bool pad_1; bool pad_2; bool pad_3; bool pad_A;
	bool pad_4; bool pad_5; bool pad_6; bool pad_B;
	bool pad_7; bool pad_8; bool pad_9; bool pad_C;
	bool pad_x; bool pad_0; bool pad_s; bool pad_D;

	digitalWrite(25, LOW);
	digitalWrite(24, HIGH);
	digitalWrite(23, HIGH);
	digitalWrite(22, HIGH);
	if (digitalRead(29) == 0)pad_1 = 1; else pad_1 = 0;
	if (digitalRead(28) == 0)pad_4 = 1; else pad_4 = 0;
	if (digitalRead(27) == 0)pad_7 = 1; else pad_7 = 0;
	if (digitalRead(26) == 0)pad_x = 1; else pad_x = 0;

	digitalWrite(25, HIGH);
	digitalWrite(24, LOW);
	digitalWrite(23, HIGH);
	digitalWrite(22, HIGH);
	if (digitalRead(29) == 0)pad_2 = 1; else pad_2 = 0;
	if (digitalRead(28) == 0)pad_5 = 1; else pad_5 = 0;
	if (digitalRead(27) == 0)pad_8 = 1; else pad_8 = 0;
	if (digitalRead(26) == 0)pad_0 = 1; else pad_0 = 0;

	digitalWrite(25, HIGH);
	digitalWrite(24, HIGH);
	digitalWrite(23, LOW);
	digitalWrite(22, HIGH);
	if (digitalRead(29) == 0)pad_3 = 1; else pad_3 = 0;
	if (digitalRead(28) == 0)pad_6 = 1; else pad_6 = 0;
	if (digitalRead(27) == 0)pad_9 = 1; else pad_9 = 0;
	if (digitalRead(26) == 0)pad_s = 1; else pad_s = 0;

	digitalWrite(25, HIGH);
	digitalWrite(24, HIGH);
	digitalWrite(23, HIGH);
	digitalWrite(22, LOW);
	if (digitalRead(29) == 0)pad_A = 1; else pad_A = 0;
	if (digitalRead(28) == 0)pad_B = 1; else pad_B = 0;
	if (digitalRead(27) == 0)pad_C = 1; else pad_C = 0;
	if (digitalRead(26) == 0)pad_D = 1; else pad_D = 0;

	if (pad_0 == 1)
	{
		if (PAD_0 == 0)
			PAD_0 = 1;
		else if (PAD_0 == 1)
			PAD_0 = 2;
	}
	else
	{
		if (PAD_0 == 1 ||
			PAD_0 == 2)
			PAD_0 = 3;
		else PAD_0 = 0;
	}

	if(	pad_1==1)
	{
		if (PAD_1 == 0)
			PAD_1 = 1;
		else if (PAD_1 == 1)
			PAD_1 = 2;
	}
	else
	{
		 if (PAD_1 == 1 || 
		 	 PAD_1 == 2)
		 	 PAD_1 = 3;
		else PAD_1 = 0;
	}

	if  (pad_2 == 1){
	 if (PAD_2 == 0)
		 PAD_2 = 1;
	 else if (PAD_2 == 1)
		 PAD_2 = 2;
	}
	else{
	 if (PAD_2 == 1 ||
		 PAD_2 == 2)
		 PAD_2 = 3;
	else PAD_2 = 0;
	}
	if (pad_3 == 1) {
		if (PAD_3 == 0)
			PAD_3 = 1;
		else if (PAD_3 == 1)
			PAD_3 = 2;
	}
	else {
		if (PAD_3 == 1 ||
			PAD_3 == 2)
			PAD_3 = 3;
		else PAD_3 = 0;
	}

	if (pad_4 == 1) {
		if (PAD_4 == 0)
			PAD_4 = 1;
		else if (PAD_4 == 1)
			PAD_4 = 2;
	}
	else {
		if (PAD_4 == 1 ||
			PAD_4 == 2)
			PAD_4 = 3;
		else PAD_4 = 0;
	}

	if (pad_5 == 1) {
		if (PAD_5 == 0)
			PAD_5 = 1;
		else if (PAD_5 == 1)
			PAD_5 = 2;
	}
	else {
		if (PAD_5 == 1 ||
			PAD_5 == 2)
			PAD_5 = 3;
		else PAD_5 = 0;
	}

	if (pad_6 == 1) {
		if (PAD_6 == 0)
			PAD_6 = 1;
		else if (PAD_6 == 1)
			PAD_6 = 2;
	}
	else {
		if (PAD_6 == 1 ||
			PAD_6 == 2)
			PAD_6 = 3;
		else PAD_6 = 0;
	}

	if (pad_7 == 1) {
		if (PAD_7 == 0)
			PAD_7 = 1;
		else if (PAD_7 == 1)
			PAD_7 = 2;
	}
	else {
		if (PAD_7 == 1 ||
			PAD_7 == 2)
			PAD_7 = 3;
		else PAD_7 = 0;
	}

	if (pad_8 == 1) {
		if (PAD_8 == 0)
			PAD_8 = 1;
		else if (PAD_8 == 1)
			PAD_8 = 2;
	}
	else {
		if (PAD_8 == 1 ||
			PAD_8 == 2)
			PAD_8 = 3;
		else PAD_8 = 0;
	}

	if (pad_9 == 1) {
		if (PAD_9 == 0)
			PAD_9 = 1;
		else if (PAD_9 == 1)
			PAD_9 = 2;
	}
	else {
		if (PAD_9 == 1 ||
			PAD_9 == 2)
			PAD_9 = 3;
		else PAD_9 = 0;
	}

	if (pad_A == 1) {
		if (PAD_A == 0)
			PAD_A = 1;
		else if (PAD_A == 1)
			PAD_A = 2;
	}
	else {
		if (PAD_A == 1 ||
			PAD_A == 2)
			PAD_A = 3;
		else PAD_A = 0;
	}

	if (pad_B == 1) {
		if (PAD_B == 0)
			PAD_B = 1;
		else if (PAD_B == 1)
			PAD_B = 2;
	}
	else {
		if (PAD_B == 1 ||
			PAD_B == 2)
			PAD_B = 3;
		else PAD_B = 0;
	}

	if (pad_C == 1) {
		if (PAD_C == 0)
			PAD_C = 1;
		else if (PAD_C == 1)
			PAD_C = 2;
	}
	else {
		if (PAD_C == 1 ||
			PAD_C == 2)
			PAD_C = 3;
		else PAD_C = 0;
	}

	if (pad_D == 1) {
		if (PAD_D == 0)
			PAD_D = 1;
		else if (PAD_D == 1)
			PAD_D = 2;
	}
	else {
		if (PAD_D == 1 ||
			PAD_D == 2)
			PAD_D = 3;
		else PAD_D = 0;
	}

	if (pad_s == 1) {
		if (PAD_s == 0)
			PAD_s = 1;
		else if (PAD_s == 1)
			PAD_s = 2;
	}
	else {
		if (PAD_s == 1 ||
			PAD_s == 2)
			PAD_s = 3;
		else PAD_s = 0;
	}

	if (pad_x == 1) {
		if (PAD_x == 0)
			PAD_x = 1;
		else if (PAD_x == 1)
			PAD_x = 2;
	}
	else {
		if (PAD_x == 1 ||
			PAD_x == 2)
			PAD_x = 3;
		else PAD_x = 0;
	}
}
KeypadClass Keypad;


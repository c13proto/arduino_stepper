// OLED.h

#ifndef _OLED_h
#define _OLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <U8glib.h>//OCED�̐ݒ�
class OLEDClass
{
 protected:


 public:
	void init();
};


extern U8GLIB_SSD1306_128X64 u8g;
extern OLEDClass OLED;

#endif


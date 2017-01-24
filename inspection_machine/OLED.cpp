// 
// 
// 

#include "OLED.h"
U8GLIB_SSD1306_128X64 u8g;
void OLEDClass::init()
{
	u8g=U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);	// I2C / TWI 
	//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);	// Fast I2C / TWI 
	u8g.setFont(u8g_font_8x13r);//oledフォントの指定
}


OLEDClass OLED;


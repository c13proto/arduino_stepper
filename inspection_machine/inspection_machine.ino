#include <U8glib.h>
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);	// Fast I2C / TWI 

#include <Keypad.h>
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


void setup()
{
  /* add setup code here */
	u8g.setFont(u8g_font_8x13r);//oledフォントの指定
	Serial.begin(9600);

}

void loop()
{
	int ad = analogRead(A8);
	//KEYPAD取得
	char key = KEY.getKey();

	//display描画
	u8g.firstPage();
	do {
		char buf[10];
		sprintf(buf, "%d", ad);//intをcharはいれつに
		u8g.drawStr(4, 20, buf);
		
	} while (u8g.nextPage());


	//PCデバッグ
	Serial.println(ad);
}

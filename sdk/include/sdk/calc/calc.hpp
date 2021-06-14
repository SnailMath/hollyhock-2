#pragma once
#include <stdint.h>

#include <sdk/os/lcd.hpp> //needed for getVramAddress and 


//Graphics stuff

extern uint16_t *vram;  //The vram pointer (this is used by routines like setPixel and has to be initialized by getVramAddress() or calc_init();
extern int width;	//width  of the screen
extern int height;	//height of the screen

void line(int x1, int y1, int x2, int y2, uint16_t color);
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t colorFill, uint16_t colorLine);
void fillScreen(uint16_t color);

inline uint16_t color(uint8_t R, uint8_t G, uint8_t B){
	return	(((R<<8) & 0b1111100000000000) |
		 ((G<<3) & 0b0000011111100000) |
		 ((B>>3) & 0b0000000000011111));
}

inline void setPixel(int x,int y, uint32_t color) {
	if(x>=0 && x < width && y>=0 && y < height)
		vram[width*y + x] = color;
}

//Stuff for Initialisation and stuff

inline void calcInit(){
	vram = LCD_GetVRAMAddress();
	LCD_GetSize(&width, &height);
	LCD_VRAMBackup();
}
inline void calcEnd(){
	LCD_VRAMRestore();
	LCD_Refresh();
}

//Stuff for the keyboard

extern "C" void getKey(uint32_t *key1, uint32_t *key2);

enum Keys1 {
	KEY_SHIFT		= 0x80000000,
	KEY_CLEAR		= 0x00020000, //The Power key
	KEY_BACKSPACE		= 0x00000080,
	KEY_LEFT		= 0x00004000,
	KEY_RIGHT		= 0x00008000,
	KEY_Z			= 0x00002000,
	KEY_POWER		= 0x00000040, //The exponent key
	KEY_DIVIDE		= 0x40000000,
	KEY_MULTIPLY		= 0x20000000,
	KEY_SUBTRACT		= 0x10000000,
	KEY_ADD			= 0x08000000,
	KEY_EXE			= 0x04000000,
	KEY_EXP			= 0x00000004,
	KEY_3			= 0x00000008,
	KEY_6			= 0x00000010,
	KEY_9			= 0x00000020,
};

enum Keys2 {
	KEY_KEYBOARD		= 0x80000000,
	KEY_UP			= 0x00800000,
	KEY_DOWN		= 0x00400000,
	KEY_EQUALS		= 0x00000080,
	KEY_X			= 0x00000040,
	KEY_Y			= 0x40000000,
	KEY_LEFT_BRACKET	= 0x00000020,
	KEY_RIGHT_BRACKET	= 0x00000010,
	KEY_COMMA		= 0x00000008,
	KEY_NEGATIVE		= 0x00000004,
	KEY_0			= 0x04000000,
	KEY_DOT			= 0x00040000,
	KEY_1			= 0x08000000,
	KEY_2			= 0x00080000,
	KEY_4			= 0x10000000,
	KEY_5			= 0x00100000,
	KEY_7			= 0x20000000,
	KEY_8			= 0x00200000,
};

inline bool testKey(uint32_t key1, uint32_t key2, Keys1 key){
	(void) key2;
	if (key1 & key) return true;
	else return false;
}

inline bool testKey(uint32_t key1, uint32_t key2, Keys2 key){
	(void) key1;
	if (key2 & key) return true;
	else return false;
}

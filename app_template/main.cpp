#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("My app name")
APP_DESCRIPTION("A short description of my app")
APP_AUTHOR("My name")
APP_VERSION("1.0.2")

extern "C"
void main() {
	calcInit(); //backup screen and init some variables

	// Put your app's code here!

	//Example for fillScreen(color);
	fillScreen(color(0,64,0));

	//Example for Debug_Printf(x,y,invert_color,0,format_string) //(small text)
	Debug_Printf(10,1,false,0,"HelloWorld%d",42);

	//Example for Debug_PrintString(string, invert_color) //(big text)
	Debug_SetCursorPosition(13,1);
	Debug_PrintString("HelloWorld",0);

	//use this command to actually update the screen 
	LCD_Refresh();

	//Example for setPixel(x,y,color)
	for (int x=0; x<256;x++){
		for (int y=0; y<256; y++){
			setPixel(50+x,250+y, color(x,y,0) );
		}
	}

	//Example for triangle(x0,y0,x1,y1,x2,y2,colorFill,colorLine);
	triangle(10,20,40,250,300,100,color(0,255,0),color(0,0,255));

	//Example for line(x1,y1,x2,y2,color);
	line(100,30,290,500,color(255,0,0));      //Use RGB color
	line(110,30,300,500,0b1111100000000000);  //Or use 565 color

	//Don't forget to do LCD_Refresh after setPixel(); line(); and triangle();
	LCD_Refresh();

	//Example for getKey
	while(true){
		uint32_t key1, key2;	//First create variables
		getKey(&key1, &key2);	//then read the keys
		if(testKey(key1, key2, KEY_CLEAR)){ //Use testKey() to test if a specific key is pressed
			break;
		}
	}

	calcEnd(); //restore screen and do stuff
}

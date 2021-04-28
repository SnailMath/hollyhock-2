#include <appdef.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("My app name 2")
APP_DESCRIPTION("A short description of my app")
APP_AUTHOR("My name")
APP_VERSION("1.0.0")

extern "C"
void main() {
    // Put your app's code here!
	Debug_SetCursorPosition(1,2);
	Debug_PrintString("HelloWorld",0);
	LCD_Refresh();
}

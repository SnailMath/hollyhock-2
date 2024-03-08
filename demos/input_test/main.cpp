#include <appdef.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>

APP_NAME("Input Test")
APP_DESCRIPTION("Test of the GetInput function.")
APP_AUTHOR("The6P4C")
APP_VERSION("1.0.0")

struct LUTEntry {
	const char *str;
	uint32_t keyCode;
};

struct LUTEntry lut[] = {
	{"KEYBOARD", 0x00B3},
	{"SHIFT", 0x00A0},
	{"BACKSPACE", 0x0097},
	{"POWER_CLEAR", 0x0080},
	{"UP", 0x0090},
	{"DOWN", 0x0091},
	{"LEFT", 0x0092},
	{"RIGHT", 0x0093},
	{"EQUALS", 0x003D},
	{"X", 0xEDB8},
	{"Y", 0xEDB9},
	{"Z", 0xEDBA},
	{"POWER", 0x005E},
	{"DIVIDE", 0x002F},
	{"OPEN_PARENTHESIS", 0x0028},
	{"7", 0x0037},
	{"8", 0x0038},
	{"9", 0x0039},
	{"TIMES", 0xEE10},
	{"CLOSE_PARENTHESIS", 0x0029},
	{"4", 0x0034},
	{"5", 0x0035},
	{"6", 0x0036},
	{"MINUS", 0x002D},
	{"COMMA", 0x002C},
	{"1", 0x0031},
	{"2", 0x0032},
	{"3", 0x0033},
	{"PLUS", 0x002B},
	{"NEGATIVE", 0x001F},
	{"0", 0x0030},
	{"DOT", 0x002E},
	{"EXP", 0x001D},
	{"EXE", 0x0094}
};

void printHeader() {
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("GetInput demo (clear=quit)", false);
}

void main() {
	struct InputEvent event;

	bool running = true;

	LCD_ClearScreen();
	printHeader();
	LCD_Refresh();

	while (running) {
		Mem_Memset(&event, 0, sizeof(event));
		GetInput(&event, 0xFFFFFFFF, 0x10);

		switch (event.type) {
		case EVENT_KEY:
			LCD_ClearScreen();

			printHeader();

			Debug_SetCursorPosition(0, 1);
			Debug_PrintString("Key Event", true);

			Debug_SetCursorPosition(0, 3);
			Debug_PrintString("Direction: ", false);

			switch (event.data.key.direction) {
			case KEY_PRESSED:
				Debug_PrintString("pressed", false);
				break;
			case KEY_HELD:
				Debug_PrintString("held", false);
				break;
			case KEY_RELEASED:
				Debug_PrintString("released", false);
				break;
			}

			Debug_SetCursorPosition(0, 4);
			Debug_PrintString("Key code: ", false);

			// Wrap in a block to avoid "jump to case label" errors due to the
			// definition of the variable i
			{
				int i = 0;
				while (true) {
					struct LUTEntry lutEntry = lut[i++];

					if (event.data.key.keyCode == lutEntry.keyCode) {
						Debug_PrintString(lutEntry.str, false);
						break;
					}
				}
			}

			LCD_Refresh();

			if (event.data.key.keyCode == KEYCODE_POWER_CLEAR) {
				running = false;
			}
			break;
		case EVENT_TOUCH:
			LCD_ClearScreen();

			printHeader();

			Debug_SetCursorPosition(0, 1);
			Debug_PrintString("Touch Event", true);

			Debug_SetCursorPosition(0, 3);
			Debug_PrintString("Direction: ", false);

			switch (event.data.touch_single.direction) {
			case TOUCH_DOWN:
				Debug_PrintString("down", false);
				break;
			case TOUCH_HOLD_DRAG:
				Debug_PrintString("hold/drag", false);
				break;
			case TOUCH_ACT_BAR:
				Debug_PrintString("act. bar", false);
				break;
			case TOUCH_UP:
				Debug_PrintString("up", false);
				break;
			}

			Debug_SetCursorPosition(0, 4);
			Debug_PrintString("x:", false);
			Debug_PrintNumberHex_Dword(event.data.touch_single.p1_x, 3, 4);

			Debug_SetCursorPosition(0, 5);
			Debug_PrintString("y:", false);
			Debug_PrintNumberHex_Dword(event.data.touch_single.p1_y, 3, 5);

			LCD_Refresh();
			break;
		case EVENT_ACTBAR_RESIZE:
		case EVENT_ACTBAR_SWAP:
		case EVENT_ACTBAR_ROTATE:
		case EVENT_ACTBAR_ESC:
		case EVENT_ACTBAR_SETTINGS:
			LCD_ClearScreen();

			printHeader();

			Debug_SetCursorPosition(0, 1);
			Debug_PrintString("Act. Bar Event", true);

			Debug_SetCursorPosition(0, 3);
			Debug_PrintString("Location: ", false);

			switch (event.type) {
			case EVENT_ACTBAR_RESIZE:
				Debug_PrintString("Resize", false);
				break;
			case EVENT_ACTBAR_SWAP:
				Debug_PrintString("Swap", false);
				break;
			case EVENT_ACTBAR_ROTATE:
				Debug_PrintString("Rotate", false);
				break;
			case EVENT_ACTBAR_ESC:
				Debug_PrintString("Esc", false);
				break;
			case EVENT_ACTBAR_SETTINGS:
				Debug_PrintString("Settings", false);
				break;
			}

			LCD_Refresh();
			break;
		}
	}
}

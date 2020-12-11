These are the most useful functions:

Exec: Abbreviation used in [exec files](exec.md)

C++: name of the function in C++

Addr./Val.: Address in ROM (OS version 2.1.2) (or value if it is not a function)

| Exec | C++ | Addr./Val. | Explanation |
| :------------- | :----------: | :----------: | -----------: |
| #C | void LCD_ClearScreen(); | 0x800394C0 | Clears the VRAM |
| #R | void LCD_Refresh(); | 0x8003733E | Copys VRAM content to the screen |
|  |  uint16_t *LCD_GetVRAMAddress(); | 0x8002E154 | Function, that returns the VRAM address |
| #V | | 0x8C000000 | The actual VRAM address (in OS version 2.1.2)|
|  | void LCD_GetSize(int *width, int *height); | 0x8002E140 | Stores the size of the screen in the variables pointed to|
| #X | | 320 | The actual width of the screen |
| #Y | | 528 | The actual height of the screen |
| #S | int Debug_SetCursorPosition(int x, int y); | 0x8002E430 | Sets the cursor for PrintString |
| #G | void Debug_GetCursorPosition(int *x, int *y); | 0x8002E448 | Gets the cursor position |
| #W | int Debug_WaitKey(); | 0x80094380 | Waits until clear is pressed (some other keys also work)|
| #PS | bool Debug_PrintString(const char *string, bool invert); | 0x8002DA0C | Prints the string at the cursor position|
| #PN | void Debug_PrintNumberHex_Nibble(uint8_t value, int x, int y); | 0x80094466 | prints a hex nibble |
| #PB | void Debug_PrintNumberHex_Byte(uint8_t value, int x, int y); | 0x800944A0 | prints a hex byte |
| #PW | void Debug_PrintNumberHex_Word(uint16_t value, int x, int y); | 0x800944C8 | prints a hex word |
| #PL / #PD | void Debug_PrintNumberHex_Dword(uint32_t value, int x, int y); | 0x80094514 | prints a hex long/dword |
| #PF | void Debug_Printf(int x, int y, bool invert, int zero, const char *format, ...); | 0x8002DBC8 | prints the format string |
|  | void LCD_VRAMBackup();  | 0x8002D3FA | Backs up the VRAM |
|  | void LCD_VRAMRestore(); | 0x8002D41A | Restores the VRAM |

You can find all functions in the .hpp files in `/sdk/include/`.

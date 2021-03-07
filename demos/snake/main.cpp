#include <appdef.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>

APP_NAME("Snake")
APP_DESCRIPTION("A simple implementation of the game Snake.")
APP_AUTHOR("The6P4C, modified by SnailMath")
APP_VERSION("1.0.0")

#define COLOR_BACKGROUND RGB_TO_RGB565(0, 0, 0)
#define COLOR_SNAKE RGB_TO_RGB565(0x1F, 0, 0)
#define COLOR_FRUIT RGB_TO_RGB565(0, 0x3F, 0)

#define BLOCK_SIZE 20
#define MAX_SNAKE_LENGTH 50
#define FRUIT_SQUARES_BONUS 3

#define DIRECTION_NORTH 0
#define DIRECTION_EAST 1
#define DIRECTION_SOUTH 2
#define DIRECTION_WEST 3

uint16_t *vram;
int lcdWidth, lcdHeight;
int numBlocksX, numBlocksY;

// the snake's head is at position 0 in our list
int snakeLength;
int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH];
int numBlocksToAdd;

int direction;

uint32_t fruitXLFSR;
uint32_t fruitYLFSR;

int fruitX;
int fruitY;

void drawBlock(int blockX, int blockY, uint16_t color) {
	for (int x = blockX * BLOCK_SIZE; x < (blockX + 1) * BLOCK_SIZE; ++x) {
		for (int y = blockY * BLOCK_SIZE; y < (blockY + 1) * BLOCK_SIZE; ++y) {
			vram[x + (y + 24) * lcdWidth] = color;
		}
	}
}

void draw() {
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("Score: ", false);

	char *num = "0000";
	num[0] = (snakeLength / 1000) % 10 + '0';
	num[1] = (snakeLength / 100) % 10 + '0';
	num[2] = (snakeLength / 10) % 10 + '0';
	num[3] = snakeLength % 10 + '0';
	Debug_PrintString(num, false);

	// Draw the background
	for (int blockY = 0; blockY < numBlocksY; ++blockY) {
		for (int blockX = 0; blockX < numBlocksX; ++blockX) {
			drawBlock(blockX, blockY, COLOR_BACKGROUND);
		}
	}

	// Draw the snake
	for (int i = 0; i < snakeLength; ++i) {
		drawBlock(snakeX[i], snakeY[i], COLOR_SNAKE);
	}

	// Draw the fruit
	drawBlock(fruitX, fruitY, COLOR_FRUIT);

	LCD_Refresh();
}

bool moveSnake() {
	int deltaX = 0;
	int deltaY = 0;

	if (direction == DIRECTION_NORTH) {
		deltaY = -1;
	} else if (direction == DIRECTION_EAST) {
		deltaX = 1;
	} else if (direction == DIRECTION_SOUTH) {
		deltaY = 1;
	} else if (direction == DIRECTION_WEST) {
		deltaX = -1;
	}

	int newX = snakeX[0] + deltaX;
	int newY = snakeY[0] + deltaY;

	// if we've gone outside the grid
	if (newX < 0 || newX >= numBlocksX || newY < 0 || newY >= numBlocksY) {
		return false;
	}

	// Move the current blocks of the snake back one to make room for the head
	for (int i = snakeLength - 1; i >= 0; --i) {
		snakeX[i + 1] = snakeX[i];
		snakeY[i + 1] = snakeY[i];
	}

	snakeX[0] = newX;
	snakeY[0] = newY;

	if (numBlocksToAdd > 0) {
		--numBlocksToAdd;
		++snakeLength;
	}

	return true;
}

void moveFruit() {
	bool foundPosition = false;

	while (!foundPosition) {
		// Emulate modulo - SH4 doesn't let us do mod by an arbitary value
		uint32_t xCopy = fruitXLFSR & 0xFF;
		while (xCopy >= numBlocksX) {
			xCopy -= numBlocksX;
		}
		fruitX = xCopy;

		uint32_t yCopy = fruitYLFSR & 0xFF;
		while (yCopy >= numBlocksY) {
			yCopy -= numBlocksY;
		}
		fruitY = yCopy;

		// calculate lfsr
		// poly: x^32 + x^22 + x^2 + x
		uint32_t fruitXLFSRBit = ((fruitXLFSR >> 0) ^ (fruitXLFSR >> 10) ^ (fruitXLFSR >> 30) ^ (fruitXLFSR >> 31)) & 1;
		fruitXLFSR = (fruitXLFSR >> 1) | (fruitXLFSRBit << 31);

		uint32_t fruitYLFSRBit = ((fruitYLFSR >> 0) ^ (fruitYLFSR >> 10) ^ (fruitYLFSR >> 30) ^ (fruitYLFSR >> 31)) & 1;
		fruitYLFSR = (fruitYLFSR >> 1) | (fruitYLFSRBit << 31);

		foundPosition = true;

		// check we haven't placed the fruit on the snake
		for (int i = 0; i < snakeLength; ++i) {
			if (fruitX == snakeX[i] && fruitY == snakeY[i]) {
				foundPosition = false;
				break;
			}
		}
	}
}

bool checkSnake() {
	bool collidedWithSelf = false;
	for (int i = 1; i < snakeLength; ++i) {
		if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
			collidedWithSelf = true;
			break;
		}
	}

	if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
		numBlocksToAdd += FRUIT_SQUARES_BONUS;
		moveFruit();
	}

	return !collidedWithSelf;
}

void main() {
	LCD_VRAMBackup();

	// Initialize our constants
	LCD_ClearScreen();

	vram = LCD_GetVRAMAddress();
	LCD_GetSize(&lcdWidth, &lcdHeight);

	numBlocksX = lcdWidth / BLOCK_SIZE;
	numBlocksY = (lcdHeight - 24) / BLOCK_SIZE;

	snakeLength = 3;
	for (int i = 0; i < snakeLength; ++i) {
		snakeX[i] = numBlocksX / 2 - i;
		snakeY[i] = numBlocksY / 2;
	}
	direction = DIRECTION_EAST;

	fruitXLFSR = 0x7312FAE1;
	fruitYLFSR = 0xAF05432A;
	moveFruit();

	struct InputEvent event;

	bool lost = false;
	bool running = true;
	while (running) {
		draw();
		
		GetInput(&event, 0xFFFFFFFF, 0x10);

		Debug_PrintNumberHex_Dword(event.type, 12, 0);
		switch (event.type) {
			case EVENT_TIMER: //Defined in sdk/os/input.hpp scince 03/07/2021

				if (!moveSnake()) {
					lost = true;
					running = false;
				}

				if (!checkSnake()) {
					lost = true;
					running = false;
				}

			case EVENT_KEY:
				switch (event.data.key.keyCode) {
				case KEYCODE_UP:
					if (direction != DIRECTION_SOUTH) {
						direction = DIRECTION_NORTH;
					}
					break;
				case KEYCODE_RIGHT:
					if (direction != DIRECTION_WEST) {
						direction = DIRECTION_EAST;
					}
					break;
				case KEYCODE_DOWN:
					if (direction != DIRECTION_NORTH) {
						direction = DIRECTION_SOUTH;
					}
					break;
				case KEYCODE_LEFT:
					if (direction != DIRECTION_EAST) {
						direction = DIRECTION_WEST;
					}
					break;
				case KEYCODE_POWER_CLEAR:
					running = false;
					break;
				}
				break;
		}
	}

	Debug_WaitKey();

	LCD_VRAMRestore();
	LCD_Refresh();
}

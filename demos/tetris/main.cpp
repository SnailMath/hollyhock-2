#include <appdef.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>
//320x520 -> 16x26 (20px x 20px) -> 12x22

APP_NAME("Tetris")
APP_DESCRIPTION("A simple implementation of the game Tetris.")
APP_AUTHOR("De_Coder")
APP_VERSION("1.0.0")

void print_score(uint32_t score){	//prints to score to the top of the screen
	Debug_SetCursorPosition(7, 0);
	Debug_PrintString("Score: ", false);

	char *num = "0000";
	num[0] = (score / 1000) % 10 + '0';
	num[1] = (score / 100) % 10 + '0';
	num[2] = (score / 10) % 10 + '0';
	num[3] = score % 10 + '0';
	Debug_SetCursorPosition(14, 0);
	Debug_PrintString(num, false);
}

uint32_t random(uint32_t rand){ //generates a pseudo random number
  rand ^= (rand << 13);
  rand ^= (rand >> 17);
  rand ^= (rand << 5);
  return rand;
}

void draw_square(unsigned int x, unsigned int y, uint8_t color_index){	//draws one square at x,y in the color given by color_index
	if(x <= 12 && y <= 22){
		x = x * 20 + 41;
		y = y * 20 + 41;
		for(int i = 0; i < 18; i++){
			for(int j = 0; j < 18; j++){
				LCD_SetPixelFromPalette(x + i, y + j, color_index);
			}
		}
	}
}

void main() {
	uint32_t rand = 0;	//used to save random numbers. the last number is used as the seed for the next number
	uint32_t score = 0;
	uint32_t wait_down = 0;		//counter for the cycles until the block moves down one field
	
	const char blocks [7] [4] [2] = {	//offsets for all 7 possible blocks
		{
			{0,0},//ox
			{1,0},//xx
			{0,1},
			{1,1}
		},
		{
			{-1,0},//x0xx
			{0,0},
			{1,0},
			{2,0}
		},
		{
			{0,1},// x0x
			{-1,0},// x
			{0,0},
			{1,0}
		},
		{
			{1,1},// x0x
			{-1,0},//  x
			{0,0},
			{1,0}
		},
		{
			{-1,1},//x0x
			{-1,0},//x
			{0,0},
			{1,0}
		},
		{
			{-1,0},//x0
			{0,0}, // xx
			{0,1},
			{1,1}
		},
		{
			{0,0},// 0x
			{1,0},//xx
			{-1,1},
			{0,1}
		}
	};
	
	struct InputEvent event;
	
	bool running = true;
	bool down = false;	//should block go down fast?
	
	uint8_t field[12] [22];
	
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < 22; j++){
			field[i] [j] = 0;
		}
	}
	
	unsigned char active_block [4] [2] = {{0,0},{0,0},{0,0},{0,0}};		//positions of all fields filled by the active block
	unsigned char pos_x = 0;	//position of the active block
	unsigned char pos_y = 0;	//position of the active block
	unsigned char rot = 0;		//rotation of the active block
	unsigned char block_type = 0;		//type of the active block
	
	LCD_VRAMBackup();

	LCD_ClearScreen();
	LCD_Refresh();
	
	
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("just push some buttons. :)", false);
	LCD_Refresh();
	
	for(int i = 0; i < 4; i++){		//get some keycodes to generate a seed for the random() function
		bool r = true;
		while(r){
			Mem_Memset(&event, 0, sizeof(event));
			GetInput(&event, 0xFFFFFFFF, 0x10);
			if(event.type == EVENT_KEY){
				if(event.data.key.direction == KEY_PRESSED){
					rand += event.data.key.keyCode;
					rand *= 100;
					r = false;
				}
			}
		}
	}
	
	LCD_ClearScreen();
	LCD_Refresh();
	
	for(int i = 0; i < 13; i++){	//draw vertical lines of the board
		for(int j = 0; j < 440; j++){
			LCD_SetPixel(i * 20 + 39, j + 40, 0);
			LCD_SetPixel(i * 20 + 40, j + 40, 0);
		}
	}
	
	for(int i = 0; i < 23; i++){	//draw horizontal lines of the board
		for(int j = 0; j < 240; j++){
			LCD_SetPixel(j + 40, i * 20 + 39, 0);
			LCD_SetPixel(j + 40, i * 20 + 40, 0);
		}
	}
	
	LCD_Refresh();
	
	rot = 0;	//get first block
	pos_x = 6;
	pos_y = 0;
	rand = random(rand);
	block_type = rand % 7;
	for(int i = 0; i < 4; i++){
		active_block[i][0] = blocks[block_type][i][0] + 6;
		active_block[i][1] = blocks[block_type][i][1];
	}
	
	print_score(0);
	
	while(running){
		Mem_Memset(&event, 0, sizeof(event));
		GetInput(&event, 0x0, 0x12);
		
		if(event.type == EVENT_KEY){
			if(event.data.key.direction == KEY_PRESSED){
				switch(event.data.key.keyCode){
					case KEYCODE_OPEN_PARENTHESIS:	//turn block left
						{
							bool e = false;		//error while rotating the block?
							rot++;
							rot %= 4;
							for(int i = 0; i < 4; i++){	//for every field of the active block
								active_block[i][0] = blocks[block_type][i][0];// get offset
								active_block[i][1] = blocks[block_type][i][1];
								for(int j = 0; j < rot; j++){ // rotate offset
									char temp = active_block[i][0];
									active_block[i][0] = -active_block[i][1];
									active_block[i][1] = temp;
								}
								active_block[i][0] += pos_x; //set to position
								active_block[i][1] += pos_y;
								
								if(active_block[i][0] > 11 || active_block[i][1] > 21){	//check if its out of the game field
									e = true;
									break;
								}else if(field[active_block[i][0]][active_block[i][1]] != 0){	//check if it hits a filled field
									e = true;
									break;
								}
							}
							
							if(e){// if a error occcured rotate back and restore active_block to its state before rotation
								rot += 3;
								rot %= 4;
								for(int i = 0; i < 4; i++){
									active_block[i][0] = blocks[block_type][i][0];
									active_block[i][1] = blocks[block_type][i][1];
									for(int j = 0; j < rot; j++){
										char temp = active_block[i][0];
										active_block[i][0] = -active_block[i][1];
										active_block[i][1] = temp;
									}
									active_block[i][0] += pos_x;
									active_block[i][1] += pos_y;
								}
							}
						}
					break;
					case KEYCODE_TIMES:	//turn block right
						{
							bool e = false;	//for everything else look at turn left ^^
							rot += 3;
							rot %= 4;
							for(int i = 0; i < 4; i++){
								active_block[i][0] = blocks[block_type][i][0];
								active_block[i][1] = blocks[block_type][i][1];
								for(int j = 0; j < rot; j++){
									char temp = active_block[i][0];
									active_block[i][0] = -active_block[i][1];
									active_block[i][1] = temp;
								}
								active_block[i][0] += pos_x;
								active_block[i][1] += pos_y;
								
								if(active_block[i][0] > 11 || active_block[i][1] > 21){
									e = true;
									break;
								}else if(field[active_block[i][0]][active_block[i][1]] != 0){
									e = true;
									break;
								}
							}
							
							if(e){
								rot++;
								rot %= 4;
								for(int i = 0; i < 4; i++){
									active_block[i][0] = blocks[block_type][i][0];
									active_block[i][1] = blocks[block_type][i][1];
									for(int j = 0; j < rot; j++){
										char temp = active_block[i][0];
										active_block[i][0] = -active_block[i][1];
										active_block[i][1] = temp;
									}
									active_block[i][0] += pos_x;
									active_block[i][1] += pos_y;
								}
							}
						}
					break;
					case KEYCODE_4:	//move block to the left
						{
							bool e = false;	//error while moving the block?
							for(int i = 0; i < 4; i++){		//for every field of the active block
								if(active_block[i][0] == 0){	//block is already at the border
									e = true;
									break;
								}else if(field[active_block[i][0] - 1][active_block[i][1]] != 0){	//field is blocked
									e = true;
									break;
								}
							}
							if(!e){		//if no error occured
								pos_x--;	//update position
								for(int i = 0; i < 4; i++){		//move the fields
									active_block[i][0]--;
								}
							}
						}
					break;
					case KEYCODE_6:	//move block to the right
						{
							bool e = false;	//for everything else look at move left
							for(int i = 0; i < 4; i++){
								if(active_block[i][0] >= 11){
									e = true;
									break;
								}else if(field[active_block[i][0] + 1][active_block[i][1]] != 0){
									e = true;
									break;
								}
							}
							if(!e){
								pos_x++;
								for(int i = 0; i < 4; i++){
									active_block[i][0]++;
								}
							}
						}
					break;
					case KEYCODE_5:		//block should go down until it hits something
						down = true;
					break;
					default:
					if (event.data.key.keyCode == KEYCODE_POWER_CLEAR){		//close the game
						running = false;
					}
					break;
				}
			}
		}
		
		for(int i = 0; i < 4; i++){	//draw fields of the active block
			draw_square(active_block[i][0], active_block[i][1], block_type);
		}
		LCD_Refresh();	//update screen
		for(int i = 0; i < 4; i++){		//draw fields of the active block in white so the block dosnt leave a colored trail
			draw_square(active_block[i][0], active_block[i][1], 7);
		}
		if((wait_down >= 16 - (score / 64)) || down){	//check if block should go down
			do{	// move down one step or if down is true move down until the block hits somthing
				wait_down = 0;	//reset the cyclecounter
				bool hit = false;	//did block hit something?
				for(int i = 0; i < 4; i++){	//for every field of the active block
					if(active_block[i][1] + 1 > 21){//check if it hits the ground
						hit = true;
						break;
					}else if(field[active_block[i][0]] [active_block[i][1] + 1]){//check if it hits a filled field
						hit = true;
						break;
					}
				}
				if(hit){	//if the block hit something
					down = false;	//block hit -> dont move down anymore
					for(int i = 0; i < 4; i++){		//redraw all fields of the active block
						draw_square(active_block[i][0], active_block[i][1], block_type);
					}
					if(pos_y == 0){		//if the block hit something directly after spawning the player has lost
						running = false;
					}
					for(int i = 0; i < 4; i++){		//set all former fields of the active block to blocked 
						field[active_block[i][0]][active_block[i][1]] = block_type + 1;
					}
					
					for(int i = 0; i < 4; i++){		//check for every field if it filled a line
						bool full_line = true;
						for(int j = 0; j < 12; j++){	//test line of the active field
							if(field[j][active_block[i][1]] == 0){
								full_line = false;
							}
						}
						if(full_line){
							score += 10;	//increment score
							for(int j = i; j < 4; j++){	//move down all active fields which are above this one
								if(active_block[i][1] > active_block[j][1]){
									active_block[j][1]++;
								}
							}
							for(int j = 0; j < 12; j++){	//for every column
								for(int k = active_block[i][1]; k > 0; k--){	//move down every field above and redraw it in the new color
									field[j][k] = field[j][k - 1];
									draw_square(j,k,(field[j][k] != 0)?(field[j][k] - 1):(7));
								}
								field[j][0] = 0;	//make top field empty
								draw_square(j,0,7);	//draw top field in white
							}
						}
					}
					
					rot = 0;// reset the blocks state and get a new one
					pos_x = 5;
					pos_y = 0;
					score++;
					rand = random(rand);
					block_type = rand % 7;
					
					for(int i = 0; i < 4; i++){		//get fields of the new block
						active_block[i][0] = blocks[block_type][i][0] + 6;
						active_block[i][1] = blocks[block_type][i][1];
					}
					print_score(score);		//print new score
				}else{	//if the didnt collide move it down one step
					pos_y++;
					for(int i = 0; i < 4; i++){
						active_block[i][1]++;
					}
				}
			}while(down);// move down until the block hits somthing
		}else{
			wait_down++;//increment cycle counter
		}
	}
	
	LCD_ClearScreen();// print score and wait
	print_score(score);
	LCD_Refresh();
	Debug_WaitKey();

	LCD_VRAMRestore();
	LCD_Refresh();
}

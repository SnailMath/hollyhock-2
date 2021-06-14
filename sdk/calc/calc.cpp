#include <sdk/calc/calc.hpp>

uint16_t *vram;
int width;
int height;

//Draw a line (bresanham line algorithm)
void line(int x1, int y1, int x2, int y2, uint16_t color){
	int8_t ix, iy;

	int dx = (x2>x1 ? (ix=1, x2-x1) : (ix=-1, x1-x2) );
	int dy = (y2>y1 ? (iy=1, y2-y1) : (iy=-1, y1-y2) );

	setPixel(x1,y1,color);
	if(dx>=dy){ //the derivative is less than 1 (not so steep)
		//y1 is the whole number of the y value
		//error is the fractional part (times dx to make it a whole number)
		// y = y1 + (error/dx)
		//if error/dx is greater than 0.5 (error is greater than dx/2) we add 1 to y1 and subtract dx from error (so error/dx is now around -0.5) 
		int error = 0;
		while (x1!=x2) {
			x1 += ix; //go one step in x direction
			error += dy;//add dy/dx to the y value.
			if (error>=(dx>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				y1+=iy;
				error-=dx;
			}
			setPixel(x1,y1,color);
		}
	}else{ //the derivative is greater than 1 (very steep)
		int error = 0;
		while (y1!=y2) { //The same thing, just go up y and look at x
			y1 += iy; //go one step in y direction
			error += dx;//add dx/dy to the x value.
			if (error>=(dy>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				x1+=ix;
				error-=dy;
			}
			setPixel(x1,y1,color);
		}
	}
}

void vline(int x, int y1, int y2, uint16_t color){ //vertical line needed for triangle()
	if (y1>y2) { int z=y2; y2=y1; y1=z;}
	for (int y=y1; y<=y2; y++)
		setPixel(x,y,color);
}

//Draw a filled triangle.
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t colorFill, uint16_t colorLine){
//Filled triangles are a lot of vertical lines.
/*                                                               -
                        a   ___________----------P3              -
       P0 _________---------              ____---                -
          ---____               _____-----                       -
               b ----___  _-----   c                             -
                        P2                                       -
The triangle has three points P0, P1 and P2 and three lines a, b and c. We go from left to right, calculating the point on a and the point on b or c and then we draw a vertical line connecting these two.
*/

	//Sort the points by x coordinate
	{
		int z;
		if(x0>x2){ z=x2; x2=x0; x0=z; z=y2; y2=y0; y0=z; }
		if(x1>x2){ z=x2; x2=x1; x1=z; z=y2; y2=y1; y1=z; }
		if(x0>x1){ z=x1; x1=x0; x0=z; z=y1; y1=y0; y0=z; }
	}

	int x = x0; //x is the variable that counts from left to right

	//Values for line a
	int ay = y0; //The point y for the current x on the line a
	int aiy; //The direction of line a
	int adx = (x2>x0 ? (       x2-x0) : (        x0-x2) );
	int ady = (y2>y0 ? (aiy=1, y2-y0) : (aiy=-1, y0-y2) );
	int aerr = 0; //The y value of a (fractional part). y is actually ay+(aerr/adx)

	//Values for line b
	int by = y0; //The point y for the current x on the line b
	int biy; //The direction of line b
	int bdx = (x1>x0 ? (       x1-x0) : (        x0-x1) );
	int bdy = (y1>y0 ? (biy=1, y1-y0) : (biy=-1, y0-y1) );
	int berr = 0;

	//Values for line c
	int cy = y1; //The point y for the current x on the line y (starting at P1)
	int ciy; //The direction of line c
	int cdx = (x2>x1 ? (       x2-x1) : (        x1-x2) );
	int cdy = (y2>y1 ? (ciy=1, y2-y1) : (ciy=-1, y1-y2) );
	int cerr = 0;

	//First draw area between a and b
	while (x<x1){
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		berr+=bdy;
		while(berr>=bdx >> 2){ //if berr/bdx >= 0.5
			berr-=bdx;
			by+=biy;
		}
		vline(x,ay,by,colorFill);
	}

	//Then draw area between a and c
	while (x<x2-1){ //we don't need x=x2, bacause x should already have the right vaue... 
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		cerr+=cdy;
		while(cerr>=cdx >> 2){ //if berr/bdx >= 0.5
			cerr-=cdx;
			cy+=ciy;
		}
		vline(x,ay,cy,colorFill);
	}

	line(x0,y0,x1,y1,colorLine);
	line(x1,y1,x2,y2,colorLine);
	line(x2,y2,x0,y0,colorLine);
}


void fillScreen(uint16_t color){
	const uint32_t size = width * height;
	for(uint32_t i = 0; i<size;i++)
		vram[i] = color;
}


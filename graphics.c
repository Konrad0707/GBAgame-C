#include <stdio.h>
#include "myLib.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
//unsigned short *videoBuffer = (unsigned short *) 0x6000000; // start of the video buffer

// Key state globals
//u16 __key_curr = 0, __key_prev = 0;

void setPixel(int row, int col, unsigned short color)
{
	// create a function that will set a pixel at the specified pixel to the color passed in.
	videoBuffer[OFFSET(row, col, 240)] = color;

}

// A function to draw a FILLED rectangle starting at (r, c)
void drawRectangle(int row, int col, int width, int height, unsigned short color)
{
	// TODO: create a function that will draw a rectangle of the specified color. 
	// Row and col should refer to the top left of the rectangle.
	for (int i = 0; i < height; i++) { // loop through each row
    	for (int j = 0; j < width; j++) { // each column
      		setPixel(row + i, j + col, color);
    	}
  	}
}

void drawRectangle3(int row, int col, int width, int height, volatile unsigned short color) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
        DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT |DMA_ON;
    }
}

void drawImage3(int row, int col, int width, int height, const u16* image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = (unsigned short*) (image + r * width);
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
        DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void clearScreen() {
	drawRectangle3(0, 0, 240, 160, 0x00000);
}

void delay(int n) {
    volatile int x = 0;
    for(int i = 0; i < n*1000; i++) {
        x++;
    }
}


void waitForVblank() {
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}
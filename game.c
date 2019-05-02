/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include "led.h"

#define inc_width 31
#define inc_height 22
#define inc_box    5
#define SplitColor WHITE
#define Split

//white lines splitting boxes
const rectangle line1V={103,105,0,LCDWIDTH};
const rectangle line2V = {211, 213, 0, LCDWIDTH};

const rectangle line1H = {0, LCDHEIGHT, 76,78};
const rectangle line2H = {0, LCDHEIGHT, 157, 159};


rectangle cells[9][9];

//Function defintions
void prepareCells();
void printSplittingLines();

int main()
{
	
	init_led();

	init_lcd();
	set_frame_rate_hz(61); /* > 60 Hz  (KPZ 30.01.2015) */
	// /* Enable tearing interrupt to get flicker free display */
	EIMSK |= _BV(INT6);
	
	/* Play the game */
	
	led_on();
	set_orientation(West);


	//rectangle random={0,0,0,0};
	int i,j;
	int top=0;
	int bottom=inc_height;
	for(i=0;top<(LCDWIDTH-inc_height);i++){//here I'm using LCDWIDTH instead of LCDHeight because I set the orientatin to west
		int left=0;
	    int right = inc_width;
		for(j=0;left<(LCDHEIGHT-inc_width);j++){//here I'm using LCDHight instead of LCDWidth because I set the orientatin to west
			rectangle random = {left, right, top, bottom};
			fill_rectangle(random, DARK_GREY);
			left += inc_width + inc_box;
			right += inc_width + inc_box;
		}
		top+=inc_height+inc_box;
		bottom+=inc_height+inc_box;
	}


void printSplittingLines(){
	//print lines splitting groups
	fill_rectangle(line1V, SplitColor);
	fill_rectangle(line2V, SplitColor);
	fill_rectangle(line1H, SplitColor);
	fill_rectangle(line2H, SplitColor);
}
void prepareCells(){
	int i, j;
	int top = 0;
	int bottom = inc_height;
	for (i = 0; top < (LCDWIDTH - inc_height); i++)
	{ //here I'm using LCDWIDTH instead of LCDHeight because I set the orientatin to west
		int left = 0;
		int right = inc_width;
		for (j = 0; left < (LCDHEIGHT - inc_width); j++)
		{ //here I'm using LCDHight instead of LCDWidth because I set the orientatin to west
			rectangle random = {left, right, top, bottom};
			fill_rectangle(random, DARK_GREY);
			left += inc_width + inc_box;
			right += inc_width + inc_box;
		}
		top += inc_height + inc_box;
		bottom += inc_height + inc_box;
	}
}

	//clear_screen();
	
}

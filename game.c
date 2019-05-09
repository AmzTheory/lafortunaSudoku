/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */
#include "os.h"
//#include "input.h"
#include <stdio.h>
#include "led.h"


#define inc_width 31
#define inc_height 22
#define inc_box    5

#define SplitColor WHITE
#define cellColor DARK_GREY
#define highlight ORANGE


//white lines splitting boxes
const rectangle line1V={103,105,0,LCDWIDTH};
const rectangle line2V = {211, 213, 0, LCDWIDTH};

const rectangle line1H = {0, LCDHEIGHT, 76,78};
const rectangle line2H = {0, LCDHEIGHT, 157, 159};

//page 19 #30
const int sampleOne[9][9] = {
	{5,0,0,7,0,0,9,0,0},
	{0,6,1,5,8,0,4,0,0},
	{0,9,0,2,0,0,0,5,1},
	{0,0,0,0,0,4,3,9,8},
	{0,0,0,8,5,1,0,0,0},
	{8,4,7,3,0,0,0,0,0},
	{4,2,0,0,0,5,0,6,0},
	{0,0,9,0,6,7,5,8,0},
	{0,0,5,0,0,8,0,0,3}
};

//i  row
//j  column
typedef struct Point
{ int i,j;
}Point;

volatile Point current;
volatile int mode=1; // 1-> navigate the grid    2->change grid value

int drawSample=1;  //1 -> draw from the sample  0-> draw from cells
int done;  //1-done 0- not finished
typedef struct Cell
{
	rectangle r;
	uint16_t color;
	int value;
} Cell;

/* Current direction of movement */
// volatile enum direction d = EAST;

//Function defintions
void prepareCells();
void printCells();
void printSplittingLines();
int check_switches(int state);
void printRectangle(Cell ce);
void navigateGrid();
void changeValues();
int checkCorrect();
void init();

	Cell cells[9][9]; //will store all cells

int main()
{
	os_init();
	os_add_task(check_switches, 100, 1);

	// led_on();
	// set_orientation(West);

	init();

}

init(){
	clear_screen();
	mode=1;
	drawSample=1;
	done=0;

	current.i = 0;
	current.j = 0;
	prepareCells();
	printSplittingLines();
	printCells();

	sei(); //enable interupt
	for (;;)
	{
	} //run forever
}

int check_switches(int state){
	if(mode==1)
		navigateGrid();
	else if (mode==2)
	    changeValues();

	
		return state;
}
void navigateGrid(){
	if (get_switch_press(_BV(SWN))) //up  change rows
	{
		clear_screen();
		if (current.i == 0)
			current.i = 8;
		else
			current.i -= 1;

		prepareCells();
		printCells();
	}

	if (get_switch_press(_BV(SWW))) //right change columns
	{
		clear_screen();
		if (current.j == 0)
			current.j = 8;
		else
			current.j -= 1;

		prepareCells();
		printCells();
	}

	if (get_switch_press(_BV(SWS))) //down  change rows
	{
		clear_screen();
		if (current.i == 8)
			current.i = 0;
		else
			current.i += 1;

		prepareCells();
		printCells();
	}

	if (get_switch_press(_BV(SWE))) //left change columns
	{
		clear_screen();
		if (current.j == 8)
			current.j = 0;
		else
			current.j += 1;

		prepareCells();
		printCells();
	}
	if (get_switch_press(_BV(SWC))) //center button
	{
		mode=2;
	}
}
void changeValues(){
	 int val=cells[current.i][current.j].value;
	if (get_switch_press(_BV(SWN))) //decrease the value
	{
		clear_screen();
		if(val==0)
			cells[current.i][current.j].value = 5;
		else if (val==1)
			cells[current.i][current.j].value =9;
		else
			cells[current.i][current.j].value -= 1;//decrement by one

		//prepareCells();
		printCells();
	}

	if (get_switch_press(_BV(SWS))) //decrease the value
	{
		clear_screen();
		if (val == 0)
			cells[current.i][current.j].value = 5;
		 if (val == 9)
			cells[current.i][current.j].value = 1;
		else
			cells[current.i][current.j].value += 1;//increment by one

		//prepareCells();
		printCells();
	}

	

	if (get_switch_press(_BV(SWC))) //center button    change to navigate mode
	{
		if(done){
			if(checkCorrect()){
				display_string_xy("well done , good Job!",LCDHEIGHT/2,100);
				init();
			}

		}

		mode=1;
		//check if done
	}
}

int checkCorrect(){
	int i,j;
	int sum;
	//check the all rows values have sume of 45 (not complete solution)
	for(i=0;i<9;i++){
		sum=0;
		for (j=0; i < 9; i++)
		{
			sum+=cells[i][j].value;
		}
		if(sum!=45)
			return 0;
	}

	return 1; //the all fo rows have sum of 45 (minimal check but will say yes for the correct solution)
}

void printSplittingLines()
{
	//print lines splitting groups
	fill_rectangle(line1V, SplitColor);
	fill_rectangle(line2V, SplitColor);
	fill_rectangle(line1H, SplitColor);
	fill_rectangle(line2H, SplitColor);
	}
	void prepareCells()
	{
		int i, j;
		int top = 0;
		int bottom = inc_height;
		for (i = 0; top < (LCDWIDTH - inc_height); i++)
		{ //here I'm using LCDWIDTH instead of LCDHeight because I set the orientatin to west
			int left = 0;
			int right = inc_width;
			for (j = 0; left < (LCDHEIGHT - inc_width); j++)
			{ //here I'm using LCDHight instead of LCDWidth because I set the orientatin to west
				rectangle rect = {left, right, top, bottom};
				//fill_rectangle(rect, DARK_GREY);
				cells[i][j].r = rect;
				if (i == current.i && j == current.j)
					cells[i][j].color = highlight;
				else
					cells[i][j].color = cellColor;

				if (drawSample)
					cells[i][j].value = sampleOne[i][j];
				
				left += inc_width + inc_box;
				right += inc_width + inc_box;
			}
			top += inc_height + inc_box;
			bottom += inc_height + inc_box;
		}
		if(drawSample)//in the first iteration we will get the result from sample and then we will relie on the changes the user make
			drawSample=0;
	}
	void printCells()
	{
		int i, j;
		done=1;
		for (i = 0; i < 9; i++)
		{
			Cell *ptr = cells[i];
			for (j = 0; j < 9; j++)
			{
				printRectangle(*ptr);
				ptr++;
			}
		}
	}
	void printRectangle(Cell c)
	{
		fill_rectangle(c.r, c.color);
		if (c.value != 0)
		{
			int x = c.r.left + 10;
			int y = c.r.top + 5;
			//later we will output the value
			char val[2];
			sprintf(val, "%d", c.value);
			display_string_xy(val, x, y);
		}else
		{
			done=0;
		}
		
	}

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

/*显示函数*/
void display();
/*鼠标消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event);

void Main() /*仅初始化执行一次*/
{
	SetWindowSize(800, 600);
    InitGraphics();        	
	
	registerMouseEvent(MouseEventProcess);/*注册鼠标消息回调函数*/

	display();
}

/* 鼠标状态 */
double mousex = 0;
double mousey = 0;
int    buttondown = 0;
char   clickedItem[128];

void MouseEventProcess(int x, int y, int button, int event)
{
	/*擦除旧的*/
     SetEraseMode(TRUE);
	 display();

	 mousex = ScaleXInches(x);/*pixels --> inches*/
	 mousey = ScaleYInches(y);/*pixels --> inches*/

	 switch (event) {
	 case BUTTON_DOWN:
		 buttondown = 1;
		 break;
	 case BUTTON_UP:
		 buttondown = 0;
		 break;
	 }

	 /*画上新的*/
     SetEraseMode(FALSE);
	 display();
}

bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

void DrawBox(double x, double y, double width, double height, char label[])
{
    MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
	if( label && strlen(label)>0 ) {
		double tw = TextStringWidth(label);
		MovePen(x+(width-tw)/2,y+height/2-3);
		DrawTextString(label);
	}
}

static double SinkMagnitude = 5;

int button(double x, double y, double w, double h,char label[])
{
	double sinkx = 0, sinky = 0;

	if( inBox(mousex,mousey,x,x+w,y,y+h) ) {
		SetPenColor("Red");
		if( buttondown ) {
			strcpy(clickedItem, label);
			sinkx =   SinkMagnitude;
			sinky = - SinkMagnitude;
		}
	}
	else
		SetPenColor("Blue");

	DrawBox(x+sinkx, y+sinky, w, h, label);

	if( strcmp(clickedItem,label)==0 && // must be clicked before
		! buttondown ) // but now mouse button is up
		return 1; 

	return 0;
}

void display()
{
	static int show_more = 0;
	int x = 10, y = 500;
	int w = 50, h = 20;

	button(x,      y, w, h, "OK");
	button(x+=100, y, w, h, "Cancel");

	if( button(x+=100, y, w, h, "Quit") )
		exit(-1);

	if( button(x=40,  y-=50, 100, h, "Show More Buttons") )
		show_more = 1;
	
	if( button(x=150,  y, 100, h, "Show Less Buttons") )
		show_more = 0;

	if( show_more ) {
		button(x=10,  y-=50, w, h, "Click Me 1");
		button(x+100, y,     w, h, "Click Me 2");
		button(x+200, y,     w, h, "Click Me 3");
		button(x,     y-=50, w, h, "Click Me 4");
		button(x+100, y,     w, h, "Click Me 5");
		button(x+200, y,     w, h, "Click Me 6");

		if( button(60, y-=50, 100, h, "Click Me to Quit Demo") )
			exit(-1);
	}
}

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "linkedlist.h"
#include <math.h>

#define BAR_WIDTH 20   // 柱图的柱子宽度
#define UNIT_HEIGHT 30 // 1分成绩的高度
#define CourseNum 3    // 课程数
#define GroupNum 4     // 分组数
#define MaxScore 5     // 满分成绩
#define ScaleLen 5     // 刻度线的长度
#define SpaceX 15      // 文字与坐标轴的水平间隔
#define SpaceY 15      // 文字与坐标轴的垂直间隔

float data[GroupNum][CourseNum] = {
	{ 3.5,  4,    4.5 },
	{ 3.7,  4.4,  4.7 },
	{ 4.5,  4.8,  4.2 },
	{ 4.5,  3.5  ,  4.0 }
};

// 以(x,y)为左下角，画一个宽度为 w，高度为 h 的矩形
void DrawBox(double x, double y, double width, double height)
{
	MovePen(x, y); 
	DrawLine(0, height);
	DrawLine(width, 0);
	DrawLine(0, -height);
	DrawLine(-width, 0);
}


// 以（x, y）为坐标轴原点，绘制柱状图
void DrawGraph(int x, int y)
{
	int graph_width = GroupNum * (CourseNum+2) * BAR_WIDTH; // 总图宽度
	int graph_height = MaxScore * UNIT_HEIGHT;              // 总图高度
	char str[16];
	int g, k, strwidth;
	// 垂直坐标轴 - y
	MovePen(x, y);
	DrawLine(0, graph_height);
	for (k = 0; k <= MaxScore; k++) {
		int yk = y + k * UNIT_HEIGHT;
		// 刻度线
		MovePen(x, yk);
		DrawLine(- ScaleLen, 0);
		// 刻度值
		sprintf(str, "%d", k);
		MovePen(x-SpaceX, yk-5);
		DrawTextString(str);
	}
	// 水平坐标轴 - x
	MovePen(x, y);
	DrawLine(graph_width, 0);
	for (k = 0; k <= GroupNum; k++) {
		int xk = x + k *  (CourseNum + 2) * BAR_WIDTH;
		MovePen(xk, y);
		DrawLine(0, - ScaleLen);
	}
	// 柱状图 - bars
	for (g = 0; g < GroupNum; g++) {
		int xg = x + g * (CourseNum + 2) * BAR_WIDTH;
		for (k = 0; k < CourseNum; k++) {
			DrawBox(xg + (k + 1) * BAR_WIDTH, y, BAR_WIDTH,
				(int)(data[g][k] * UNIT_HEIGHT));
		}
		// 组名称
		sprintf(str, "Group %c", "ABCD"[g]);
		MovePen(xg + ((CourseNum + 2) * BAR_WIDTH - TextStringWidth(str))/2, y - SpaceY);
		DrawTextString(str);
	}
}

void Main() /*仅初始化执行一次*/
{
	SetWindowSize(800, 600);
	InitGraphics();/*图形初始化*/
	SetPenColor("Black"); /*设置画笔颜色*/
    SetPenSize(1);        /*设置画笔粗细*/

	DrawBox(100,100,100,100);
	return;

	DrawGraph(100,100);
}


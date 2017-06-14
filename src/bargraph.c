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

#define BAR_WIDTH 20   // ��ͼ�����ӿ��
#define UNIT_HEIGHT 30 // 1�ֳɼ��ĸ߶�
#define CourseNum 3    // �γ���
#define GroupNum 4     // ������
#define MaxScore 5     // ���ֳɼ�
#define ScaleLen 5     // �̶��ߵĳ���
#define SpaceX 15      // �������������ˮƽ���
#define SpaceY 15      // ������������Ĵ�ֱ���

float data[GroupNum][CourseNum] = {
	{ 3.5,  4,    4.5 },
	{ 3.7,  4.4,  4.7 },
	{ 4.5,  4.8,  4.2 },
	{ 4.5,  3.5  ,  4.0 }
};

// ��(x,y)Ϊ���½ǣ���һ�����Ϊ w���߶�Ϊ h �ľ���
void DrawBox(double x, double y, double width, double height)
{
	MovePen(x, y); 
	DrawLine(0, height);
	DrawLine(width, 0);
	DrawLine(0, -height);
	DrawLine(-width, 0);
}


// �ԣ�x, y��Ϊ������ԭ�㣬������״ͼ
void DrawGraph(int x, int y)
{
	int graph_width = GroupNum * (CourseNum+2) * BAR_WIDTH; // ��ͼ���
	int graph_height = MaxScore * UNIT_HEIGHT;              // ��ͼ�߶�
	char str[16];
	int g, k, strwidth;
	// ��ֱ������ - y
	MovePen(x, y);
	DrawLine(0, graph_height);
	for (k = 0; k <= MaxScore; k++) {
		int yk = y + k * UNIT_HEIGHT;
		// �̶���
		MovePen(x, yk);
		DrawLine(- ScaleLen, 0);
		// �̶�ֵ
		sprintf(str, "%d", k);
		MovePen(x-SpaceX, yk-5);
		DrawTextString(str);
	}
	// ˮƽ������ - x
	MovePen(x, y);
	DrawLine(graph_width, 0);
	for (k = 0; k <= GroupNum; k++) {
		int xk = x + k *  (CourseNum + 2) * BAR_WIDTH;
		MovePen(xk, y);
		DrawLine(0, - ScaleLen);
	}
	// ��״ͼ - bars
	for (g = 0; g < GroupNum; g++) {
		int xg = x + g * (CourseNum + 2) * BAR_WIDTH;
		for (k = 0; k < CourseNum; k++) {
			DrawBox(xg + (k + 1) * BAR_WIDTH, y, BAR_WIDTH,
				(int)(data[g][k] * UNIT_HEIGHT));
		}
		// ������
		sprintf(str, "Group %c", "ABCD"[g]);
		MovePen(xg + ((CourseNum + 2) * BAR_WIDTH - TextStringWidth(str))/2, y - SpaceY);
		DrawTextString(str);
	}
}

void Main() /*����ʼ��ִ��һ��*/
{
	SetWindowSize(800, 600);
	InitGraphics();/*ͼ�γ�ʼ��*/
	SetPenColor("Black"); /*���û�����ɫ*/
    SetPenSize(1);        /*���û��ʴ�ϸ*/

	DrawBox(100,100,100,100);
	return;

	DrawGraph(100,100);
}


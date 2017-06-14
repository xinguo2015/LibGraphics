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

static char text[80]; /*������ַ���������*/
static int textlen = 0;/*������ַ�������*/
static double textx, texty; /*�ַ�������ʼλ��*/

void CharEventProcess(char c);/*�ַ���Ϣ�ص�����*/

void Main() /*����ʼ��ִ��һ��*/
{
	SetWindowSize(800, 800);
    InitGraphics();        	
	
	registerCharEvent(CharEventProcess);/*ע���ַ���Ϣ�ص�����*/

	SetPenColor("Red"); 
    SetPenSize(1);

	textx = 0;
	texty = GetWindowHeight() - 20;
	MovePen(textx, texty);
	DrawTextString("Input whatever you want");
	MovePen(textx, texty-=20);
	DrawTextString("ESC --- exit");
	MovePen(textx, texty-=20);
	DrawTextString("Backspace --- delte the last char");
	MovePen(textx, texty-=20);
	DrawTextString("Return --- go to next line");
	MovePen(textx, texty-=20);

	SetPenColor("Blue"); 
}


void CharEventProcess(char c)
{
     switch (c) {
	 case VK_ESCAPE: /*ESC*/
		 exit(-1);
		 break;
	 case VK_RETURN:  /* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
		   textlen = 0;
		   text[textlen] = '\0';
		   texty -= 20; // next line
		   break;
	 case VK_BACK:
		   if (textlen > 0) {
			   // erase old
			   SetEraseMode(TRUE); 
			   MovePen(textx, texty);
			   DrawTextString(text);
			   text[--textlen] = '\0';
			   // draw new
			   SetEraseMode(FALSE);
			   MovePen(textx, texty);
			   DrawTextString(text);
		   }
		   break;
      default:
		  if (textlen < 79) {
			  /*����ǰ�ַ����뵽�����ַ���������*/
			  text[textlen++] = c;
			  text[textlen] = '\0';
			  // ��ʾ
			  MovePen(textx, texty);
			  DrawTextString(text);
		  }
	 	   break;
    }
}

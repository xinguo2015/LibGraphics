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

static char text[80]; /*输入的字符串缓冲区*/
static int textlen = 0;/*输入额字符串长度*/
static double textx, texty; /*字符串的起始位置*/

void CharEventProcess(char c);/*字符消息回调函数*/

void Main() /*仅初始化执行一次*/
{
	SetWindowSize(800, 800);
    InitGraphics();        	
	
	registerCharEvent(CharEventProcess);/*注册字符消息回调函数*/

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
	 case VK_RETURN:  /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
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
			  /*将当前字符加入到整个字符缓冲区中*/
			  text[textlen++] = c;
			  text[textlen] = '\0';
			  // 显示
			  MovePen(textx, texty);
			  DrawTextString(text);
		  }
	 	   break;
    }
}

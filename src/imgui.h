#ifndef ________i_m_g_u_i_____h_____________________
#define ________i_m_g_u_i_____h_____________________

#include <stdlib.h>
#include <stdio.h>

#define IMGUI_ESC             27
#define IMGUI_BACKSPACE       8
#define IMGUI_RETURN          '\r'
#define IMGUI_KEY_TAB         '\t'

// special keys
#define IMGUI_KEY_LEFT          VK_LEFT
#define IMGUI_KEY_UP            VK_UP
#define IMGUI_KEY_RIGHT         VK_RIGHT
#define IMGUI_KEY_DOWN          VK_DOWN

//===========================================================================

// 
// Generate a *fake* unique ID for gui controls at compiling/run time
//
//   [L:16-1][N:16-1]
//        XOR
//   [F:32 --------1]
//

#define GenUIID(N) ( ((__LINE__<<16) | ( N & 0xFFFF))^((long)&__FILE__) )

//
// GenUIID(0) will give a unique ID at each source code line. 
// If you need one UI ID per line, just call GenUIID with 0
//
//               GenUIID(0)
//
// But, two GenUIID(0) calls at the same line will give the same ID.
// So, in a while/for loop body, GenUIID(0) will give you the same ID.
// In this case, you need call GenUIID with different N parameter: 
//
//               GenUIID(N)
//
//===========================================================================

typedef struct { 
	int mousex;     // x - mouse position
	int mousey;     // y - mouse position
	int buttondown; // 1 - down, 0 - otherwise
	int clickxy[2]; // where clicked down
	int wheel;      // 1 and -1 for up/down wheel direction

	int hotitem;    // item below the mouse cursor
	int activeitem; // item under interaction

	int key;        // key pressed
	int keychar;    // char input
	int modifier;   // modifier flag: Ctrl, Alt, Shift
	int lastkeys[3];// last key, keychar, modifier

	int kboarditem; // item with keyboard focus
	int lastkbitem; // last item with keyboard focus

	int textHeight; // hight of text display
} ImGUIState;

// standard routines
void imgui_init();
void imgui_prepare();
void imgui_finish();	

// event processing
void imgui_onKeyboardUp(unsigned char key, int modifier, int x, int y);
void imgui_onKeyboard(unsigned char key, int modifier, int x, int y);
void imgui_onSpecialUp(unsigned char key, int modifier, int x, int y);
void imgui_onSpecial(unsigned char key, int modifier, int x, int y);
void imgui_onMouse(int button, int state, int x, int y);
void imgui_onMotion(int x, int y);

// widgets
int imgui_button   (int id, int x, int y, int w, int h, char label[]);
int imgui_checkbox (int id, int x, int y, int w, int h, char label[], int *value);
int imgui_radio    (int id, int x, int y, int w, int h, char label[], int reference, int *value);
int imgui_listbox  (int id, int x, int y, int w, int h, char*items[], int nitem, int *liststart, int *value);
int imgui_slider   (int id, int x, int y, int w, int h, double minv, double maxv, double delta, double * value);
int imgui_slider_f (int id, int x, int y, int w, int h, float  minv, float maxv, float delta, float * value);
int imgui_slider_i (int id, int x, int y, int w, int h, int minv, int maxv, int delta, int * value);
int imgui_editbox  (int id, int x, int y, int w, int h, char textbuf[], int maxbuf);
int imgui_textlabel(int id, int x, int y, int w, int h, char text[]);
int imgui_frame    (int id, int x, int y, int w, int h);

#endif // ifndef ________i_m_g_u_i_____h_____________________
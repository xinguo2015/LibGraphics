#include "graphics.h"
#include "imgui.h"

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

static int getTicks();
static int hitRect(int x, int y, int w, int h);
static int listitem(int id, int x, int y, int w, int h, char label[], int selected);
static int slider_base(int id, int x, int y, int w, int h, double minv, double maxv, double delta, double * value);

static void drawRect(int x, int y, int w, int h, unsigned int color);
static void drawCrossX(int x, int y, int w, int h, unsigned int color);
static void fillRect(int x, int y, int w, int h, unsigned int color);
static void fillDiamond(int x, int y, int w, int h, unsigned int color);
static void drawCircle(float cx, float cy, float r, int num_segments, unsigned int color);
static void drawText(const char text[], int x, int y, unsigned int color, void * font);
static void drawTextAfter(const char text[], unsigned int color, void * font);

static int guiButtonSink  = 2;
static int guiColorStill  = 0xCCBBAA;
static int guiColorHot    = 0xFFDDCC;
static int guiColorCheck  = 0xFFEEDD;
static int guiColorWhite  = 0xFFFFFF;
static int guiColorFocus  = 0xFFEEDD;
static int guiColorEdit   = 0x550000;
static int guiColorPicked = 0xFFEEDD;
static int guiColorLabel  = 0x333333;

static ImGUIState gGuiState;

static int AlignY(int H, int h)
{
	return (H-h)/2+4;
}


void imgui_init()
{
	memset(&gGuiState, 0, sizeof(gGuiState));
}

void imgui_prepare()
{
	gGuiState.hotitem = 0;
}

void imgui_finish()
{
	if (gGuiState.buttondown == 0)
	{
		gGuiState.activeitem = 0;
	}
	// If no widget grabbed tab, clear focus
	if (gGuiState.key == IMGUI_KEY_TAB)
		gGuiState.kboarditem = 0;
	// Clear the entered key
	gGuiState.key = 0;
	gGuiState.keychar = 0;
}

void imgui_onKeyboard(unsigned char key, int modifier, int x, int y)
{
	gGuiState.lastkeys[0] = gGuiState.key      = key;
	gGuiState.lastkeys[1] = gGuiState.keychar  = key;
	gGuiState.lastkeys[2] = gGuiState.modifier = modifier;
}

void imgui_onSpecial(unsigned char key, int modifier, int x, int y)
{
	gGuiState.lastkeys[1] = gGuiState.key      = key;
	gGuiState.lastkeys[2] = gGuiState.modifier = modifier;
}

void imgui_onKeyboardUp(unsigned char key, int modifier, int x, int y)
{
	gGuiState.modifier = modifier;
}

void imgui_onSpecialUp(unsigned char key, int modifier, int x, int y)
{
	gGuiState.modifier = modifier;
}

void imgui_onMouse(int button, int state, int x, int y)
{
	if ( button == LEFT_BUTTON ) 
	{	//这里只处理鼠标**左**键
		gGuiState.buttondown = (state==BUTTON_DOWN);
		gGuiState.mousex = x;
		gGuiState.mousey = y;
		if( gGuiState.buttondown ) {
			//printf("mousedown (%d, %d)\n", x, y);
			gGuiState.clickxy[0] = x;
			gGuiState.clickxy[1] = y;
		}
	}
	else if ( button==3 )
		gGuiState.wheel = state ? 1 : 0;
	else if ( button==4 )
		gGuiState.wheel = state ? -1 : 0;
}

void imgui_onMotion(int x, int y)
{
	gGuiState.mousex = x;
	gGuiState.mousey = y;
}

// Check whether current mouse position is within a rectangle
int imgui_hitRect(int x, int y, int w, int h)
{
	return (gGuiState.mousex > x && gGuiState.mousey > y &&
		gGuiState.mousex < x + w && gGuiState.mousey < y + h);
}

// Simple button IMGUI widget
int imgui_button(int id, int x, int y, int w, int h, char label[])
{
	int alignX = 0;
	int alignY = AlignY(h, gGuiState.textHeight);
	// Check whether the button should be hot
	if (hitRect(x, y, w, h))
	{
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}
	// Draw button 
	if (gGuiState.hotitem == id)    {
		// button is hot
		if ( ! gGuiState.buttondown ) {
			// Button is merely 'hot', 绘制颜色为guiColorHot
			fillRect(x, y, w, h, guiColorHot);
			// 添加按钮的标签
			drawText(label, x+alignX, y+alignY,guiColorLabel, NULL);
		} else {
			// mouse is down，将按钮的位置进行稍许偏移，增加动感
			fillRect(x+guiButtonSink, y-guiButtonSink, w, h, guiColorHot);
			// 添加按钮的标签
			drawText(label, x+alignX+guiButtonSink, y-guiButtonSink+alignY, guiColorLabel, NULL);
		}
	} else {
		// button is not hot
		fillRect(x, y, w, h, guiColorStill);
		drawText(label, x+alignX, y+alignY, guiColorLabel, NULL);
	}

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gGuiState.hotitem == id && 
		gGuiState.activeitem == id &&
		gGuiState.buttondown == 0 )
	{
		return 1;
	}

	// Otherwise, no clicky (click and release).
	return 0;
}

int imgui_checkbox (int id, int x, int y, int w, int h, char label[], int *value)
{
	int bs = w<h ? w : h;
	int textAlignY = AlignY(h, gGuiState.textHeight);
	// Check whether the button should be hot
	if (hitRect(x, y, w, h))
	{
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}
	// Draw checkbox button
	fillRect(x, y, bs, bs, gGuiState.hotitem == id ? guiColorHot : guiColorStill);
	drawText(label, x+bs+4, y+textAlignY, guiColorLabel, NULL);
	if ( *value ) // is checked, then draw a cross
		drawCrossX(x+1, y+1, bs-2, bs-2, guiColorCheck);

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gGuiState.hotitem == id && 
		gGuiState.activeitem == id &&
		gGuiState.buttondown == 0 )
	{
		*value = !(*value);
		return 1;
	}

	return 0;
}


void imgui_radioFrame(int x, int y, int w, int h)
{
	drawRect(x,y,w,h,guiColorHot);
}

int imgui_radio(int id, int x, int y, int w, int h, char label[], int reference, int *value)
{
	int bs = w<h ? w : h;
	int textAlignY = AlignY(h, gGuiState.textHeight);
	// Check whether the button should be hot
	if (hitRect(x, y, w, h))
	{
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}
	// Draw radio button
	fillRect(x,y, bs, bs, gGuiState.hotitem == id ? guiColorHot : guiColorStill);
	drawText(label, x+bs+4, y+textAlignY, guiColorLabel, NULL);
	if ( reference == *value )
	{
		//fillRect(x+bs/4,y+bs/4, bs/2, bs/2, guiColorCheck);
		//drawCircle(x+bs/2,y+bs/2, bs/4, 16, guiColorCheck);
		//fillDiamond(x+bs/4,y+bs/4, bs/2, bs/2, guiColorCheck);
		fillDiamond(x+bs/6,y+bs/6, bs*2/3, bs*2/3, guiColorCheck);
	}

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gGuiState.hotitem == id && 
		gGuiState.activeitem == id &&
		gGuiState.buttondown == 0 )
	{
		if( *value != reference )
		{
			*value = reference;
			return 1;
		}
	}

	return 0;
}

// Simple scroll bar IMGUI widget
int imgui_slider(int id, int x, int y, int w, int h, double min, double max, double delta, double * value)
{
	int cursize = 16; // cursor size
	int border = 2;   // distance against the cursor
	int hintsize = 2; // thickness of the focus hint
	int vertical = w < h;// sliding direction
	double posratio = CLAMP((*value - min)/(max-min), 0, 1);
	int curpos;

	w = CLAMP( w, cursize+border*2, w);
	h = CLAMP( h, cursize+border*2, h);
	if( vertical ) h = h<cursize*4 ? cursize*4 : h;
	else           w = w<cursize*4 ? cursize*4 : w;

	curpos = (int)( posratio * ((vertical?h:w) - border*2 - cursize) ) + border;

	// Check for hotness
	if (hitRect(x, y, w, h)) {
		gGuiState.kboarditem = id;
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}

	// If no widget has keyboard focus, take it
	if (gGuiState.kboarditem == 0)
		gGuiState.kboarditem = id;

	// render the bar
	if (gGuiState.kboarditem == id) {
		fillRect(x, y, w, h, guiColorFocus );
		fillRect(x+hintsize, y+hintsize, w-hintsize*2, h-hintsize*2, guiColorStill);
	}
	else {
		fillRect(x, y, w, h, guiColorStill );
	}

	// render the cursor
	if (gGuiState.activeitem == id || gGuiState.hotitem == id) {
		fillRect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorWhite);
	} else {
		fillRect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorHot);
	}
	// If we have keyboard focus, we'll need to process the keys

	if (gGuiState.kboarditem == id)
	{
		switch (gGuiState.key)
		{
		case IMGUI_KEY_TAB:
		case IMGUI_RETURN:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			gGuiState.kboarditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (gGuiState.modifier & IMGUI_SHIFT)
				gGuiState.kboarditem = gGuiState.lastkbitem;
			// Also clear the key so that next widget
			// won't process it
			gGuiState.key = 0;
			break;
		case IMGUI_KEY_DOWN:
		case IMGUI_KEY_LEFT:
			// Slide slider up (if not at zero)
			if (*value > min) {
				(*value) = CLAMP(*value - delta, min, max);
				return 1;
			}
			break;
		case IMGUI_KEY_UP:
		case IMGUI_KEY_RIGHT:
			// Slide slider down (if not at max)
			if (*value < max) {
				(*value) = CLAMP(*value + delta, min, max);
				return 1;
			}
			break;
		default:
			if( gGuiState.wheel>0 && *value<max) {
				(*value) = CLAMP(*value + delta, min, max);
				gGuiState.wheel = 0;
				return 1;
			}
			else if( gGuiState.wheel<0 && *value>min) {
				(*value) = CLAMP(*value - delta, min, max);
				gGuiState.wheel = 0;
				return 1;
			}
		}

	}

	gGuiState.lastkbitem = id;

	// Update widget value
	if (gGuiState.activeitem == id) {
		double newvalue = vertical ? 
			(gGuiState.mousey - (y + border + cursize/2))/(double)(h-border*2-cursize) :
		(gGuiState.mousex - (x + border + cursize/2))/(double)(w-border*2-cursize) ;
		newvalue = min + CLAMP(newvalue,0,1)*(max-min);
		gGuiState.kboarditem = id; // let it accept keyboard
		if (*value != newvalue ) {
			*value = newvalue;
			return 1;
		}
	}

	return 0;
}

int imgui_slider(int id, int x, int y, int w, int h, float min, float max, float delta, float * value)
{
	double v = *value;
	if( slider(id, x, y, w, h, (double)min, (double)max, (double)delta, & v) ) {
		*value = (float)v;
		return 1;
	}
	return 0;
}	

int imgui_slider(int id, int x, int y, int w, int h, int min, int max, int delta, int * value)
{
	double v = *value;
	if( slider(id, x, y, w, h, (double)min, (double)max, (double)delta, & v) ) {
		*value = (int)(v+0.1);
		return 1;
	}
	return 0;
}

int imgui_editbox(int id, int x, int y, int w, int h, char textbuf[], int maxbuf)
{
	int alignX = 5, alignY = AlignY(h, gGuiState.textHeight);
	int len = strlen(textbuf);
	int finishEditting = 0;
	// Check whether the button should be hot
	if (hitRect(x, y, w, h))
	{
		gGuiState.kboarditem = id;
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}
	// If no widget has keyboard focus, take it
	if (gGuiState.kboarditem == 0)
		gGuiState.kboarditem = id;
	// If we have keyboard focus, show it
	if (gGuiState.kboarditem == id)
		fillRect(x-1, y-1, w+2, h+2, guiColorFocus);

	// Render the text box 
	if ( gGuiState.hotitem == id || gGuiState.activeitem == id ) {
		// 'hot' or 'active'
		fillRect(x, y, w, h, guiColorHot);
	} else {
		fillRect(x, y, w, h, guiColorStill   );
	}

	// show text
	drawText(textbuf, x+alignX, y+alignY, guiColorEdit, NULL);
	// Render cursor if we have keyboard focus
	if ( gGuiState.kboarditem == id && ( getTicks() >> 18) & 1)
		drawText("_", guiColorEdit, NULL);

	// If we have keyboard focus, we'll need to process the keys
	if (gGuiState.kboarditem == id)
	{
		switch (gGuiState.key)
		{
		case IMGUI_RETURN:
			gGuiState.key = 0;
			finishEditting = 1;
			break;
		case IMGUI_KEY_TAB:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			gGuiState.kboarditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (gGuiState.modifier & IMGUI_SHIFT)
				gGuiState.kboarditem = gGuiState.lastkbitem;
			// Also clear the key so that next widget
			// won't process it
			gGuiState.key = 0;
			break;
		case IMGUI_BACKSPACE:
			if( len > 0 ) {
				textbuf[--len] = 0;
				//textChanged = 1;
			}
			gGuiState.key = 0;
			break;
		}
		if (gGuiState.keychar >= 32 && gGuiState.keychar < 127 && len < maxbuf ) {
			textbuf[len] = gGuiState.keychar;
			textbuf[++len] = 0;
			//textChanged = 1;
		}
	}

	gGuiState.lastkbitem = id;

	// If button is hot and active, but mouse button is not
	// down, the user must have clicked the button.
	if (   gGuiState.buttondown == 0 
		&& gGuiState.hotitem == id 
		&& gGuiState.activeitem == id )
		gGuiState.kboarditem = id;
	return finishEditting;
}

int imgui_textlabel(int id, int x, int y, int w, int h, char text[])
{
	drawText(text, x, y+AlignY(h, gGuiState.textHeight), guiColorLabel, NULL);
	return 0;
}

int imgui_listbox (int id, int x, int y, int w, int h, char*items[], int nitem, int *firstitem, int *selection)
{
	int needslider = 0;
	int nShow, k, witems = w;
	int newSelection = *selection;
	int listItemHeight = gGuiState.textHeight+5;

	nShow = (h-4) / listItemHeight;
	nShow = CLAMP( nShow, 1, nitem); 
	if( nShow<nitem ) {
		needslider = 1;
		witems -= 22;
	}

	if( needslider && hitRect(x,y,w,h) )
		gGuiState.kboarditem = 0; // force slider take keyboard

	fillRect(x, y, w, h,guiColorStill);
	int slidervalue = nitem-*firstitem;
	if( needslider && slider(id+GenUIID(0), x+witems, y+2, 
		w-witems-2, h-4, nShow-1, nitem, 1, &slidervalue) ) {
			*firstitem = nitem-(int)(slidervalue+0.1);
	}

	drawRect(x,  y,  w,  h,  0x77777777);

	for( k = 0; k<nShow; k++ ) {
		int iid = needslider ? k + *firstitem : k;
		if( iid<nitem && listitem(id+GenUIID(k), x+2, y+h-3-(k+1)*listItemHeight, 
			witems-4, listItemHeight, items[iid], iid==*selection) )
			newSelection = iid;
	}

	if( needslider )
		// a separator to fill the gap
		fillRect(x+witems-2, y, 2, h, 0x77777777);

	if( *selection != newSelection ) {
		*selection = newSelection	;
		return 1;
	}
	return 0;
}

int imgui_listitem(int id, int x, int y, int w, int h, char label[], int selected)
{
	int alignY = AlignY(h, gGuiState.textHeight);
	if (hitRect(x, y, w, h))
	{
		gGuiState.hotitem = id;
		if (gGuiState.activeitem == 0 && gGuiState.buttondown)
			gGuiState.activeitem = id;
	}

	if (gGuiState.hotitem == id	&& gGuiState.buttondown )
		selected = 1;

	if (gGuiState.hotitem == id)
		fillRect(x, y, w, h, guiColorHot);
	else if ( selected )
		fillRect(x, y, w, h, guiColorPicked);
	else
		fillRect(x, y, w, h, guiColorStill);
	drawText(label, x+5, y+alignY, guiColorLabel, NULL);

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gGuiState.hotitem == id && 
		gGuiState.activeitem == id &&
		gGuiState.buttondown == 0 )
	{
		return 1;
	}

	// Otherwise, no clicky.
	return 0;
}

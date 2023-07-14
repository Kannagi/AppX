#include "demo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "AppX.h"


#include <GL/glew.h>
#include "nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"

#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

int hexToInt(const std::string& hexValue)
{
	int intValue;
	std::stringstream ss;
	if(hexValue[0] == '#')
		ss << std::hex << hexValue.substr(1);
	else
		ss << std::hex << hexValue;



	ss >> intValue;
	return intValue | (0xFF<<24);
}

int isBlack(NVGcolor col)
{
	if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
	{
		return 1;
	}
	return 0;
}

static char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
	case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
	case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
	case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
	case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
	case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
	case 1: str[0] = cp;
	}
	return str;
}




static	DemoData data;
int loadDemoData(NVGcontext* vg, DemoData* data)
{
	int i;

	if (vg == NULL)
		return -1;

	for (i = 0; i < 12; i++) {
		char file[128];
		snprintf(file, 128, "data/images/image%d.jpg", i+1);
		data->images[i] = nvgCreateImage(vg, file, 0);
		if (data->images[i] == 0) {
			printf("Could not load %s.\n", file);
			return -1;
		}
	}

	data->fontIcons = nvgCreateFont(vg, "icons", "data/entypo.ttf");
	if (data->fontIcons == -1) {
		printf("Could not add font icons.\n");
		return -1;
	}
	data->fontNormal = nvgCreateFont(vg, "sans", "data/Roboto-Regular.ttf");
	if (data->fontNormal == -1) {
		printf("Could not add font italic.\n");
		return -1;
	}
	data->fontBold = nvgCreateFont(vg, "sans-bold", "data/Roboto-Bold.ttf");
	if (data->fontBold == -1) {
		printf("Could not add font bold.\n");
		return -1;
	}
	data->fontEmoji = nvgCreateFont(vg, "emoji", "data/NotoEmoji-Regular.ttf");
	if (data->fontEmoji == -1) {
		printf("Could not add font emoji.\n");
		return -1;
	}
	nvgAddFallbackFontId(vg, data->fontNormal, data->fontEmoji);
	nvgAddFallbackFontId(vg, data->fontBold, data->fontEmoji);

	return 0;
}


void drawEditBoxBase(NVGcontext* vg, float x, float y, float w, float h)
{
	NVGpaint bg;
	// Edit
	bg = nvgBoxGradient(vg, x+1,y+1+1.5f, w-2,h-2, 3,4, nvgRGBA(255,255,255,32), nvgRGBA(32,32,32,32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, 4-1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, 4-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);
}

void drawButton(NVGcontext* vg, int preicon, const WIDGET_BSS &widget,const STYLE_BSS &style)
{
	NVGpaint bg;
	float cornerRadius = style.border_radius;

	unsigned char r = style.background_color;
	unsigned char g = style.background_color>>8;
	unsigned char b = style.background_color>>16;
	unsigned char a = style.background_color>>24;

	unsigned char lr = style.background_color_linear;
	unsigned char lg = style.background_color_linear>>8;
	unsigned char lb = style.background_color_linear>>16;
	unsigned char la = style.background_color_linear>>24;

	nvgBeginPath(vg);

	//nvgRGBA(0,240*0.5,240,255), nvgRGBA(0,120*0.5,120,255));

	int x = widget.rect.x;
	int y = widget.rect.y;
	int w = widget.rect.w;
	int h = widget.rect.h;

	bg = nvgLinearGradient(vg, x,y,x+0,y+h/1.5, nvgRGBA(r,g,b,a), nvgRGBA(lr,lg,lb,la));

	nvgRoundedRect(vg, x+1,y+1, w-1,h-1, cornerRadius);

	nvgFillPaint(vg, bg);
	nvgFill(vg);

    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
	nvgStroke(vg);

	//text
	nvgFontSize(vg, style.font_size);
	nvgFontFace(vg, style.font_ttf.c_str());
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);



	nvgFillColor(vg, nvgRGBA(0,0,0,196));
	nvgText(vg, x+w*0.5f+style.font_shadow.x,y+h*0.5f+style.font_shadow.y,widget.text.c_str(), NULL);

	nvgFillColor(vg, nvgRGBA(220,220,220,255));
	nvgText(vg, x+w*0.5f,y+h*0.5f,widget.text.c_str(), NULL);



	//nvgFillColor(vg, nvgRGBA(0,0,0,64));
	//nvgFill(vg);
}


void drawSpinner(NVGcontext* vg, float cx, float cy, float r, float t)
{
	float a0 = 0.0f + t*6;
	float a1 = NVG_PI + t*6;
	float r0 = r;
	float r1 = r * 0.75f;
	float ax,ay, bx,by;
	NVGpaint paint;

	nvgSave(vg);

	nvgBeginPath(vg);
	nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
	nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
	nvgClosePath(vg);
	ax = cx + cosf(a0) * (r0+r1)*0.5f;
	ay = cy + sinf(a0) * (r0+r1)*0.5f;
	bx = cx + cosf(a1) * (r0+r1)*0.5f;
	by = cy + sinf(a1) * (r0+r1)*0.5f;
	paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgRGBA(0,0,0,0), nvgRGBA(0,0,0,128));
	nvgFillPaint(vg, paint);
	nvgFill(vg);

	nvgRestore(vg);
}

void drawEditBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{

	drawEditBoxBase(vg, x,y, w,h);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);
}

void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint;
	NVGpaint headerPaint;
nvgBeginPath(vg);



    //nvgScissor(vg, x,y,w/2,h/2);
	//

	//nvgTranslate(vg, 0, -100);

	// Window
// Drop shadow

	shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	//nvgBeginPath(vg);
	nvgRect(vg, x-10,y-10, w+20,h+30);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);


    nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28,30,34,192));
	//nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgFill(vg);


	nvgStrokeColor(vg, nvgRGBA(0,0,0,128));
	nvgStroke(vg);




	float stackh = 1000,t = 10.0f;
	int i;
	float u = (1+cosf(t*0.5f))*0.5f;
	float u2 = (1-cosf(t*0.2f))*0.5f;
	float scrollh = 0.0f;

shadowPaint = nvgBoxGradient(vg, x+w-12+1,y+4+1, 8,h-8, 3,4, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+w-12,y+4, 8,h-8, 3);
	//nvgFillPaint(vg, shadowPaint);
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	nvgFill(vg);

	scrollh = (h/stackh) * (h-8);
	shadowPaint = nvgBoxGradient(vg, x+w-12-1,y+4+(h-8-scrollh)*u-1, 8,scrollh, 3,4, nvgRGBA(220,220,220,255), nvgRGBA(128,128,128,255));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+w-12+1,y+4+1 + (h-8-scrollh)*u, 8-2,scrollh-2, 2);
	//nvgFillPaint(vg, shadowPaint);
	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);


	// Header
	headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
	nvgFillPaint(vg, headerPaint);
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, x+0.5f, y+0.5f+30);
	nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
	nvgStroke(vg);

	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg,2);
	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgText(vg, x+w/2,y+16+1, title, NULL);

	nvgFontBlur(vg,0);
	nvgFillColor(vg, nvgRGBA(220,220,220,160));
	nvgText(vg, x+w/2,y+16, title, NULL);

}

static float clampf(float a, float mn, float mx) { return a < mn ? mn : (a > mx ? mx : a); }
void drawThumbnails(NVGcontext* vg, float x, float y, float w, float h, int nimages, float t)
{
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint, imgPaint, fadePaint;
	float ix,iy,iw,ih;
	float thumb = 60.0f;
	float arry = 30.5f;
	int imgw, imgh;
	float stackh = (nimages/2) * (thumb+10) + 10;
	int i;
	float u = (1+cosf(t*0.5f))*0.5f;
	float u2 = (1-cosf(t*0.2f))*0.5f;
	float scrollh, dv;

	nvgSave(vg);
//	nvgClearState(vg);

	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x,y+4, w,h, cornerRadius*2, 20, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(vg);
	nvgRect(vg, x-10,y-10, w+20,h+30);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	// Window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgMoveTo(vg, x-10,y+arry);
	nvgLineTo(vg, x+1,y+arry-11);
	nvgLineTo(vg, x+1,y+arry+11);
	nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgFill(vg);

	nvgSave(vg);
	nvgScissor(vg, x,y,w,h);
	nvgTranslate(vg, 0, -(stackh - h)*u);

	dv = 1.0f / (float)(nimages-1);

	for (i = 0; i < nimages; i++) {
		float tx, ty, v, a;
		tx = x+10;
		ty = y+10;
		tx += (i%2) * (thumb+10);
		ty += (i/2) * (thumb+10);
		//nvgImageSize(vg, images[i], &imgw, &imgh);
		if (imgw < imgh) {
			iw = thumb;
			ih = iw * (float)imgh/(float)imgw;
			ix = 0;
			iy = -(ih-thumb)*0.5f;
		} else {
			ih = thumb;
			iw = ih * (float)imgw/(float)imgh;
			ix = -(iw-thumb)*0.5f;
			iy = 0;
		}

		v = i * dv;
		a = clampf((u2-v) / dv, 0, 1);

		if (a < 1.0f)
			drawSpinner(vg, tx+thumb/2,ty+thumb/2, thumb*0.25f, t);
/*
		imgPaint = nvgImagePattern(vg, tx+ix, ty+iy, iw,ih, 0.0f/180.0f*NVG_PI, images[i], a);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, tx,ty, thumb,thumb, 5);
		nvgFillPaint(vg, imgPaint);
		nvgFill(vg);
*/
		shadowPaint = nvgBoxGradient(vg, tx-1,ty, thumb+2,thumb+2, 5, 3, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
		nvgBeginPath(vg);
		nvgRect(vg, tx-5,ty-5, thumb+10,thumb+10);
		nvgRoundedRect(vg, tx,ty, thumb,thumb, 6);
		nvgPathWinding(vg, NVG_HOLE);
		nvgFillPaint(vg, shadowPaint);
		nvgFill(vg);

		nvgBeginPath(vg);
		nvgRoundedRect(vg, tx+0.5f,ty+0.5f, thumb-1,thumb-1, 4-0.5f);
		nvgStrokeWidth(vg,1.0f);
		nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
		nvgStroke(vg);
	}
	nvgRestore(vg);

	// Hide fades
	fadePaint = nvgLinearGradient(vg, x,y,x,y+6, nvgRGBA(200,200,200,255), nvgRGBA(200,200,200,0));
	nvgBeginPath(vg);
	nvgRect(vg, x+4,y,w-8,6);
	nvgFillPaint(vg, fadePaint);
	nvgFill(vg);

	fadePaint = nvgLinearGradient(vg, x,y+h,x,y+h-6, nvgRGBA(200,200,200,255), nvgRGBA(200,200,200,0));
	nvgBeginPath(vg);
	nvgRect(vg, x+4,y+h-6,w-8,6);
	nvgFillPaint(vg, fadePaint);
	nvgFill(vg);

	// Scroll bar
	shadowPaint = nvgBoxGradient(vg, x+w-12+1,y+4+1, 8,h-8, 3,4, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+w-12,y+4, 8,h-8, 3);
	nvgFillPaint(vg, shadowPaint);
//	nvgFillColor(vg, nvgRGBA(255,0,0,128));
	nvgFill(vg);

	scrollh = (h/stackh) * (h-8);
	shadowPaint = nvgBoxGradient(vg, x+w-12-1,y+4+(h-8-scrollh)*u-1, 8,scrollh, 3,4, nvgRGBA(220,220,220,255), nvgRGBA(128,128,128,255));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+w-12+1,y+4+1 + (h-8-scrollh)*u, 8-2,scrollh-2, 2);
	nvgFillPaint(vg, shadowPaint);
//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);

	nvgRestore(vg);
}

void drawSearchBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = h/2-1;

	// Edit
	bg = nvgBoxGradient(vg, x,y+1.5f, w,h, h/2,5, nvgRGBA(0,0,0,16), nvgRGBA(0,0,0,92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

/*	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);*/

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.55f, y+h*0.55f, cpToUTF8(ICON_SEARCH,icon), NULL);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,32));

	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*1.05f,y+h*0.5f,text, NULL);

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,32));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h*0.55f, y+h*0.55f, cpToUTF8(ICON_CIRCLED_CROSS,icon), NULL);
}

void drawDropDown(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;

	bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h*0.5f, y+h*0.5f, cpToUTF8(ICON_CHEVRON_RIGHT,icon), NULL);
}

void drawLabel(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(200,200,200,255));

	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x,y+h*0.5f,text, NULL);
}



void drawEditBoxNum(NVGcontext* vg,const char* text, const char* units, float x, float y, float w, float h)
{
	float uw;

	drawEditBoxBase(vg, x,y, w,h);

	uw = nvgTextBounds(vg, 0,0, units, NULL, NULL);

	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h*0.3f,y+h*0.5f,units, NULL);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-uw-h*0.5f,y+h*0.5f,text, NULL);
}

void drawCheckBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	//NVG_NOTUSED(w);

	nvgBeginPath(vg);

	bg = nvgBoxGradient(vg, x+1,y+(int)(h*0.5f)-9+1, 18,18, 3,3, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));

	nvgRoundedRect(vg, x+1,y+(int)(h*0.5f)-9, 18,18, 3);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
return;
	nvgFontSize(vg, 33);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+9+2, y+h*0.5f, cpToUTF8(ICON_CHECK,icon), NULL);
}

void drawSlider(NVGcontext* vg, float pos, float x, float y, float w, float h)
{
	NVGpaint bg, knob;
	float cy = y+(int)(h*0.5f);
	float kr = (int)(h*0.25f);

	// Slot
	nvgBeginPath(vg);
	bg = nvgBoxGradient(vg, x,cy-2+1, w,4, 2,2, nvgRGBA(60,60,60,255), nvgRGBA(30,30,30,255));
	nvgRoundedRect(vg, x,cy-2, w,4, 2);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	// Knob
	knob = nvgLinearGradient(vg, x,y,x,y+h/1.5, nvgRGBA(100,100,100,255), nvgRGBA(60,60,60,255));
	nvgBeginPath(vg);
	nvgCircle(vg, x+(int)(pos*w),cy, kr-1);
	nvgFillPaint(vg, knob);
	nvgFill(vg);

	nvgCircle(vg, x+(int)(pos*w),cy, kr-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,92));
	nvgStroke(vg);
}

//--------------------------------------------------------------------------


static std::string get_file(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::string content;

    if(file)
    {
        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        content.resize(file_size);

        if (file.read(&content[0], file_size))
        {
            return content;
        }
        else
        {
            std::cout << "Error read file !" << std::endl;
        }

        file.close();
    }
    else
    {
        std::cout << "Error file open !" << std::endl;
    }

    return content;
}

SUI_Widget::SUI_Widget(std::vector<std::string> &strFileStyle)
{
	this->vg = nvgCreateGL2(NVG_ANTIALIAS );
	this->width = 640;
	this->height = 480;
	this->wradio = 1.0;
	this->visible = true;
	this->type = -1;

	NVGcontext* vg = (NVGcontext* )this->vg;

	loadDemoData(vg,&data);

	std::string file_str;

	for (const std::string& file_name : strFileStyle)
    {
        const std::string file_str = get_file(file_name);

        this->push(file_str,"button",0,0);

        int i = 0;
        while(this->push(file_str,"button-layout",1,i) == true)
			i++;
        //std::cout << file_str << std::endl;

        //std::cout << test <<std::endl;

    }

    for (Widget_BSS& widget : this->widget)
    {
        for (Style_BSS& style : this->style)
        {
            if(style.name == widget.name)
            {
               widget.style = &style.bss;
            }
        }
    }


	//file_str = get_file();


	//this->search(text,"button",0);
}


std::string SUI_Widget::getNextWord(const std::string& text,std::string::size_type &in_start)
{
    const std::string WORD = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_0123456789";
    std::string word;
    std::string::size_type start = text.find_first_of(WORD,in_start);

    if (start == std::string::npos)
        return word;

    std::string::size_type end = text.find_first_not_of(WORD,start);

    if (end != std::string::npos)
    {
        word = text.substr(start, end - start);
    }
    else
    {
        word = text.substr(start);
    }

    return word;
}

enum
{
    SUI_BUTTON,
    SUI_LABEL,
    SUI_TEXT,

    SUI_UNKNOW = -1,

};

static int getwidget(const std::string& element)
{
    if(element == "button")
        return SUI_BUTTON;

    if(element == "button-active")
        return SUI_BUTTON;

    if(element == "button-hover")
        return SUI_BUTTON;

    if(element == "button-layout")
        return SUI_BUTTON;

    return SUI_UNKNOW;
}


bool SUI_Widget::push(const std::string& text, const std::string& element,const int type,int loop)
{
    std::string::size_type start = text.find(element);

	if (start == std::string::npos)
        return false;

	for(int i = 0;i < loop;i++)
	{
		if (start != std::string::npos)
			start = text.find(element, start + element.length());
		else
			return false;
	}

    start = text.find('.', start);

    if (start == std::string::npos)
        return false;

    std::string word = getNextWord(text,start);

    Style_BSS style;
    Widget_BSS widget;

    style.name  = word;
    widget.name = word;

    //std::cout << word << "\n";

    //----------------
    std::string::size_type closeToken;
    start = text.find('{', start);
    closeToken = text.find('}', start);

    std::string words = text.substr(start + 1, closeToken - start - 1);
    //std::cout << words << "\n";

    //----------------

    std::array<std::string,40> key_style = {
    "font-ttf", "font-size", "font-color","border-radius","background-color",
    "brightness","background-color-linear",

    "text",

    "visible",

    "font-shadow",

    "rect","background-color-rgba","background-color-linear-rgba",
    };


    int index = 0;
    for(int i = 0;i < key_style.size();i++)
    {
        if(key_style[i] == "")
            continue;

        start = words.find(key_style[i]);

        if (start == std::string::npos)
            continue;

        std::string keyword = getNextWord(words,start);

        //std::cout << keyword << "\n";

        start = words.find('(', start);
        closeToken = words.find(')', start);

        if (closeToken == std::string::npos)
            continue;

        std::string args = words.substr(start + 1, closeToken - start - 1);
        //std::cout << index << " : ";
        //std::cout << args << "\n";

        const char *arg= args.c_str();

        std::replace(args.begin(), args.end(), ',', ' ');

        int result[4];
        float fresult[4];

        std::stringstream ss(args);
        for (int l = 0; l < 4; ++l)
            ss >> result[l];

        for (int l = 0; l < 4; ++l)
            ss >> fresult[l];

        if(keyword == "rect")
        {
            widget.bss.rect.x = result[0];
            widget.bss.rect.y = result[1];
            widget.bss.rect.w = result[2];
            widget.bss.rect.h = result[3];
        }

        if(keyword == "text")
        {
            widget.bss.text = args;
        }

        if(keyword == "visible")
        {
            if(args == "false")
                widget.visible = false;
        }

        if(keyword == "font-ttf")
            style.bss.font_ttf = args;

        if(keyword == "font-size")
            style.bss.font_size = atof(arg);

        if(keyword == "font-color")
            style.bss.font_color = hexToInt(args);

        if(keyword == "border-radius")
            style.bss.border_radius = atof(arg);

        if(keyword == "background-color")
            style.bss.background_color = hexToInt(args);

        if(keyword == "background-color-rgba")
        {
            style.bss.background_color = result[0] | (result[1]<<8) | (result[2]<<16) | (result[3]<<24);
        }

        if(keyword == "background-color-linear-rgba")
        {
            style.bss.background_color_linear = result[0] | (result[1]<<8) | (result[2]<<16) | (result[3]<<24);
        }

        if(keyword == "brightness")
            style.bss.font_ttf = atof(arg);

        if(keyword == "font-shadow")
        {
            style.bss.font_shadow.x = result[0];
            style.bss.font_shadow.y = result[1];
        }


        //std::cout << "Element " << index << ": " << tableau[index] << std::endl;
        index++;
    }

    if(type == 0)
    {
        this->style.push_back(style);
    }else
    {
        widget.type = getwidget(element);
        this->widget.push_back(widget);
    }

	return true;
}

void SUI_Widget::render()
{
	NVGcontext* vg = (NVGcontext* )this->vg;
	nvgBeginFrame(vg, this->width, this->height, this->wradio);

    for (const Widget_BSS& widget : this->widget)
    {
        if(widget.visible == false)
            continue;


        const STYLE_BSS* ptr = widget.style;

        const STYLE_BSS& style = *ptr;


        switch(widget.type)
        {
            case SUI_BUTTON :
                drawButton(vg, ICON_LOGIN,  widget.bss,style);
            break;

            case SUI_LABEL :

            break;

            case SUI_TEXT :

            break;
        }


    }

    //drawWindow(vg, "Widgets `n Stuff", 50, 50, 300, 400);
	//drawDropDown(vg, "Effects", 100,20,280,28);
	drawLabel(vg, "Login", 20,20, 280,20);

	drawSlider(vg, 1.0f, 100,50, 170,28);

	//drawSearchBox(vg, "Search", 300,50,280,25);

	drawCheckBox(vg, "Remember me", 100,150, 140,28);

	drawEditBox(vg, "Password", 300,150, 280,28);

    nvgEndFrame(vg);

    //drawThumbnails(vg, 365, 30, 160, 300, 12, 0);

	return;

    //drawButton(vg, ICON_LOGIN, "Sign in", 138, 20, 120, 24, nvgRGBA(0,96,128,255));

    //drawThumbnails(vg, 365, 30, 160, 300, 12, 0);

	float x = 20;

	float y = 20;

    // Widgets
	drawWindow(vg, "Widgets `n Stuff", 50, 50, 300, 400);
	x = 60; y = 95;
	drawSearchBox(vg, "Search", x,y,280,25);
	y += 40;
	drawDropDown(vg, "Effects", x,y,280,28);
	y += 45;

	// Form
	drawLabel(vg, "Login", x,y, 280,20);
	y += 25;
	drawEditBox(vg, "Email",  x,y, 280,28);
	y += 35;
	drawEditBox(vg, "Password", x,y, 280,28);
	y += 38;
	drawCheckBox(vg, "Remember me", x,y, 140,28);
	//drawButton(vg, ICON_LOGIN, "Sign in", x+138, y, 140, 28, nvgRGBA(0,96,128,255));
	y += 45;

	// Slider
	drawLabel(vg, "Diameter", x,y, 280,20);
	y += 25;
	drawEditBoxNum(vg, "123.00", "px", x+180,y, 100,28);
	drawSlider(vg, 1.0f, x,y, 170,28);


}

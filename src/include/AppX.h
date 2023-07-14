
#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef struct
{
    float x,y,w,h;

}Vec4f;

typedef struct
{
    int x,y,w,h;

}Vec4i;

typedef struct
{
    float x,y;

}Vec2f;

typedef struct
{
    int x,y;

}Vec2i;

typedef struct
{
    std::string font_ttf;

    float font_size;
    int font_color;
    Vec2i font_shadow;

    int background_color,background_color_linear;
    float border_radius;

    bool padding_type;
    Vec2i padding;
    Vec2f fpadding;

    float brightness,grayscale;

}STYLE_BSS;

typedef struct
{
    std::string text;

    bool rect_type;

    Vec4i rect;
    Vec4f frect;

}WIDGET_BSS;



class Style_BSS
{
private:

public:
    std::string name;

    STYLE_BSS bss;

    Style_BSS();
};

class Widget_BSS
{
private:

public:
    std::string name;
    int type;
    bool visible;
    int z_order;

    WIDGET_BSS bss;
    STYLE_BSS *style;

    Widget_BSS();
};


typedef struct
{
    int delay_time;
    bool loop,tar;

    std::vector<std::string> strFileModule;
    std::vector<std::string> strFileGlobal;
    std::vector<std::string> strFileStyle;
}APPX_FILE;

class SUI_Widget
{
private:
	void *vg;
	int width,height,type;
	float wradio;
	bool visible;
	std::vector<Style_BSS> style;
	std::vector<Widget_BSS> widget;

	std::string getNextWord(const std::string& text,std::string::size_type &in_start);
	void search(const std::string& text, const std::string& element,const int type);

public:
    SUI_Widget(std::vector<std::string> &strFileStyle);
    bool push(const std::string& text, const std::string& element,const int type,int loop);
    void render();
};


std::vector<std::string> extractNameFileJSON(const std::string& filejson,const std::string& groupname);
std::map<std::string, std::string> extractValues(const std::string& jsonFile);

int enginejs(APPX_FILE &appxf);


void UI_init();
void UI_Window(const char *title,int w,int h);
void UI_FlipBuffer();
void UI_Sleep(int time_ms);
void UI_Clear();


#define EVENT_MAX 512
typedef struct
{
    unsigned char key[EVENT_MAX];
    unsigned char clikright,clikleft,exit;
    int mousex,mousey;

}Event;
void Event_Init(Event &event);
void Event_Update(Event *event);

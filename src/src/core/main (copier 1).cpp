#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AppX.h"


#include <GL/glew.h>
#include "nanovg.h"



extern "C" {
    void init_nanovg();
    void render_nanovg(int winWidth,int winHeight,float pxRatio,int blowup,float t,float mx,float my);
    NVGcontext* nvgCreateGL2(int flags);
}

int main2();

int main()
{
/*
    //main2();
    //return 0;
	UI_init();

	UI_Window("AppX",640,480);


	if(glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}




    Event event;
    Event_Init(event);

    int winWidth = 640, winHeight = 480;
    float pxRatio;
    pxRatio = 1.0;


    init_nanovg();
    //SUI_Widget widget;

    double t = 0;


    while( (event.exit == 0) )
    {

        Event_Update(&event);
        UI_Clear();

        //widget.render();

        render_nanovg(640,480,1.0,event.clikleft,t,event.mousex,event.mousey);

        UI_FlipBuffer();
        UI_Sleep(33);
        t += 0.05;
    }



	return 0;
*/
    UI_init();

    const std::string fileJSON = "appx.json";

    APPX_FILE appxf;
    appxf.strFileGlobal = extractNameFileJSON(fileJSON,"global");
    appxf.strFileModule = extractNameFileJSON(fileJSON,"module");
    appxf.strFileStyle  = extractNameFileJSON(fileJSON,"style");

    std::map<std::string, std::string> values = extractValues(fileJSON);
/*
    // Display the values
    std::cout << "Name: " << values["name"] << std::endl;
    std::cout << "Version: " << values["version"] << std::endl;
	std::cout << "tar: " << values["tar"] << std::endl;
*/
    enginejs(appxf);

    return 0;
}



#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <sstream> // Ajout de cette ligne

static std::string getNextWord(const std::string& text,std::string::size_type &in_start)
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




static void search(const std::string& text, const std::string& element)
{
    std::array<int, 4> result;
    std::string::size_type start = text.find(element);

    if (start == std::string::npos)
        return;

    start = text.find('.', start);

    if (start == std::string::npos)
        return;

    std::string word = getNextWord(text,start);
    //std::cout << word << "\n";

    //----------------
    std::string::size_type closeToken;
    start = text.find('{', start);
    closeToken = text.find('}', start);

    std::string words = text.substr(start + 1, closeToken - start - 1);
    //std::cout << words << "\n";

    //----------------

    std::array<std::string,7> arr = {
    "font-ttf", "font-size", "font-color","border-radius","background-color",
    "brightness","brightness-color"
    };

    int index = 0;
    for(int i = 0;i < arr.size();i++)
    {
        if(arr[i] == "")
            continue;

        start = words.find(arr[i]);

        if (start == std::string::npos)
            continue;

        start = words.find('(', start);
        closeToken = words.find(')', start);

        if (closeToken == std::string::npos)
            continue;

        std::string args = words.substr(start + 1, closeToken - start - 1);
        std::cout << index << " : ";
        std::cout << args << "\n";
        //std::cout << "Element " << index << ": " << tableau[index] << std::endl;
        index++;
    }

/*
    std::string::size_type openParenthesis = text.find('(', start);
    std::string::size_type closeParenthesis = text.find(')', openParenthesis);

    if (! (openParenthesis != std::string::npos && closeParenthesis != std::string::npos && closeParenthesis > openParenthesis) )
        return result;

    std::string values = text.substr(openParenthesis + 1, closeParenthesis - openParenthesis - 1);
    std::replace(values.begin(), values.end(), ',', ' ');

    std::cout << values << "\n";
/*
    std::stringstream ss(values);
    for (int i = 0; i < 4; ++i)
    {
        ss >> result[i];
    }
*/
}

static char* loadFile(const char* filename,int &fileSize)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error open file : %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize <= 0)
    {
        fprintf(stderr, "File Empty : %s\n", filename);
        fclose(file);
        return NULL;
    }

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Error file memory\n");
        fclose(file);
        return NULL;
    }

    int bytesRead = fread(buffer, 1, fileSize, file);
    if(bytesRead != fileSize)
    {
        fprintf(stderr, "Error read file : %s\n", filename);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[fileSize] = '\0';

    fclose(file);

    return buffer;
}

int main2()
{
    int n;
    char* buffer = loadFile("style.bss",n);

    std::string element = "button.";
    search(buffer, element);


    //std::cout << hexToInt("#000000");

    std::cout << std::endl;

    return 0;
}

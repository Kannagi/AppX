#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>
#include "quickjs/quickjs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AppX.h"

class JSFunctionCaller
{

private:
    JSContext *ctx;
    JSValue global_obj;
    std::vector<JSValue> func_vals;

public:
    JSFunctionCaller(JSContext *ctx,JSValue global_obj)
    {
        this->ctx = ctx;
        this->global_obj = JS_GetGlobalObject(this->ctx);
    }

    ~JSFunctionCaller()
    {
        for (size_t i = 0; i < func_vals.size(); i++)
        {
            JS_FreeValue(ctx, func_vals[i]);
        }
    }

    void initCallFunction(const char *function_name)
    {
        JSValue func_val = JS_GetPropertyStr(ctx, global_obj, function_name);
        this->func_vals.push_back(func_val);
    }

    void callFunction(int id)
    {
        if (JS_IsFunction(ctx, func_vals[id]))
        {
            JSValue ret_val = JS_Call(ctx, func_vals[id], global_obj, 0, NULL);
/*
            if (JS_IsException(ret_val))
            {
                // js_std_dump_error(ctx);
            }
*/
            JS_FreeValue(ctx, ret_val);
        }
        else
        {
            std::cout << "Function '" << id << "' is not defined." << std::endl;
        }
    }
};

typedef struct
{
    int delay_time;
    bool loop;
} APPX;

APPX Appx;

static void init()
{
    Appx.delay_time = 0;
    Appx.loop = true;
}

//------------------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------------------
static JSValue appx_callback_plugin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    for (int i = 0; i < argc; i++)
    {
        const char *arg = JS_ToCString(ctx, argv[i]);
        printf("%s",arg);
        if(i != argc-1)
            printf(" , ");
        JS_FreeCString(ctx, arg);
    }

    printf("\n");

    return JS_UNDEFINED;

    //return JS_NewInt32(ctx, 5);
}

static JSValue appx_callback_print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    for (int i = 0; i < argc; i++)
    {
        const char *arg = JS_ToCString(ctx, argv[i]);
        printf("%s",arg);
        if(i != argc-1)
            printf(" , ");
        JS_FreeCString(ctx, arg);
    }

    printf("\n");

    return JS_UNDEFINED;

    //return JS_NewInt32(ctx, 5);
}

static JSValue appx_callback_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if(argc == 1)
    {
        const char *arg = JS_ToCString(ctx, argv[0]);
        Appx.delay_time = atoi(arg);

    }
    return JS_UNDEFINED;
}

static JSValue appx_callback_delay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}


static JSValue appx_callback_exit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if(argc == 1)
    {
        const char *arg = JS_ToCString(ctx, argv[0]);
        int ret = atoi(arg);
        exit(ret);

    }

    exit(0);
    return JS_UNDEFINED;
}

static JSValue appx_callback_Window(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
	const char *arg[3];
	int i;


	for(i = 0;i < 3;i++)
		arg[i] = NULL;

	for(i = 0;i < argc;i++)
		arg[i] = JS_ToCString(ctx, argv[i]);

	int w = 640;
	int h = 480;

	if(arg[1] != NULL)
		w = atoi(arg[1]);

	if(arg[2] != NULL)
		h = atoi(arg[2]);

	if(arg[0] != NULL)
		UI_Window(arg[0],w,h);
	else
		UI_Window("AppX",w,h);


	UI_Clear();


    return JS_UNDEFINED;
}

static JSValue appx_callback_WindowIcon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    if(argc == 1)
    {
        //const char *arg = JS_ToCString(ctx, argv[0]);
        //add icon
    }

    return JS_UNDEFINED;
}


static JSValue appx_callback_WidgetInput(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}


static JSValue appx_callback_WidgetButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_WidgetText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}




static JSValue appx_callback_SpriteTile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SpriteArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SpriteBitmapFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SpriteRender(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}



static JSValue appx_callback_LoadSoundPlay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SoundPlay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SoundPause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_SoundStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

static JSValue appx_callback_GetSound(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}
/*

static JSValue appx_callback_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{

    return JS_UNDEFINED;
}

*/


//-----------------------------------------------------




//-----------------------------------------------------

typedef JSValue (*JSFunction)(JSContext*, JSValue, int, JSValue*);

static inline void addFunction(JSContext *ctx,JSValue my_obj,JSFunction func,const char *name)
{
    JS_SetPropertyStr(ctx, my_obj, name, JS_NewCFunction(ctx, func, name, 0));
}

static void Eval(JSContext *ctx,const char *filename,int type)
{
    int n = 0;
    char* buffer = loadFile(filename,n);
    JSValue result = JS_Eval(ctx, buffer, n, filename, type);

    if (JS_IsException(result))
    {
        // Récupération de l'exception
        JSValue exception_val = JS_GetException(ctx);

        // Récupération des détails de l'erreur
        const char *exception_str = JS_ToCString(ctx, exception_val);
        JSValue stack_trace = JS_GetPropertyStr(ctx, exception_val, "stack");
        const char *stack_trace_str = JS_ToCString(ctx, stack_trace);

        // Affichage des détails de l'erreur
        printf("JavaScript Error:\n");
        printf("Message: %s\n", exception_str);
        printf("Stack Trace:%s\n", stack_trace_str);

        // Libération des ressources
        JS_FreeCString(ctx, exception_str);
        JS_FreeCString(ctx, stack_trace_str);
        JS_FreeValue(ctx, stack_trace);
        JS_FreeValue(ctx, exception_val);
    }

    JS_FreeValue(ctx, result);
    free(buffer);
}


int enginejs(APPX_FILE &appxf)
{
    init();

    JSRuntime *rt;
    JSContext *ctx;

    // Création du runtime QuickJS
    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);

    // Création de l'objet JavaScript et enregistrement des deux fonctions de rappel
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue my_obj = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, global_obj, "Appx", my_obj);

    addFunction(ctx,my_obj,appx_callback_plugin,"plugin");

    addFunction(ctx,my_obj,appx_callback_print,"print");
    addFunction(ctx,my_obj,appx_callback_start,"start");
    addFunction(ctx,my_obj,appx_callback_exit,"exit");
    addFunction(ctx,my_obj,appx_callback_delay,"delay");

    addFunction(ctx,my_obj,appx_callback_Window,"Window");
    addFunction(ctx,my_obj,appx_callback_WindowIcon,"WindowIcon");

    addFunction(ctx,my_obj,appx_callback_SpriteTile,"SpriteTile");
    addFunction(ctx,my_obj,appx_callback_SpriteArray,"SpriteArray");
    addFunction(ctx,my_obj,appx_callback_SpriteBitmapFont,"SpriteBitmapFont");
    addFunction(ctx,my_obj,appx_callback_SpriteRender,"SpriteRender");

    addFunction(ctx,my_obj,appx_callback_LoadSoundPlay,"LoadSoundPlay");
    addFunction(ctx,my_obj,appx_callback_SoundPlay,"SoundPlay");
    addFunction(ctx,my_obj,appx_callback_SoundPause,"SoundPause");
    addFunction(ctx,my_obj,appx_callback_SoundStop,"SoundStop");
    addFunction(ctx,my_obj,appx_callback_GetSound,"GetSound");

    addFunction(ctx,my_obj,appx_callback_WidgetInput,"WidgetInput");
    addFunction(ctx,my_obj,appx_callback_WidgetButton,"WidgetButton");
    addFunction(ctx,my_obj,appx_callback_WidgetText,"WidgetText");


    for (const std::string& file : appxf.strFileGlobal)
    {
        Eval(ctx, file.c_str(), JS_EVAL_TYPE_GLOBAL);
    }

    for (const std::string& file : appxf.strFileModule)
    {
        Eval(ctx, file.c_str(), JS_EVAL_TYPE_MODULE);
    }

    auto callback = new JSFunctionCaller(ctx,global_obj);

    callback->initCallFunction("global_Appx_main"); //0
    callback->initCallFunction("global_Appx_update"); //1
    callback->initCallFunction("global_Appx_event"); //2
    callback->initCallFunction("global_Appx_view"); //3


    callback->callFunction(0); //global_Appx_main


    //std::chrono::milliseconds duration(Appx.delay_time);

    int time_ms = Appx.delay_time;
    int time_msVsync = 0;

    Event event;
    Event_Init(event);

    if(Appx.delay_time > 0)
    {
        SUI_Widget widget(appxf.strFileStyle);

        while( (Appx.loop == true) && (event.exit == 0) )
        {
            time_ms+=5;
            if(Appx.delay_time <= time_ms)
            {
                callback->callFunction(1); //global_Appx_update
                time_ms -= Appx.delay_time;
            }

            if(time_msVsync >= 60)
            {
                Event_Update(&event);
                UI_Clear();

                widget.render();
                //callback->callFunction(2); //event
                //callback->callFunction(3); //render
				UI_FlipBuffer();
				time_msVsync -= 60;
            }
            time_msVsync+= 5;

            UI_Sleep(5);
        }
    }

    delete callback;

    JS_FreeValue(ctx, my_obj);
    JS_FreeValue(ctx, global_obj);
    JS_FreeContext(ctx);
    //JS_FreeRuntime(rt);


    return 0;
}

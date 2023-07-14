
#include <SDL2/SDL.h>

#include <stdio.h>
#include <GL/glew.h>
static SDL_Window *Window;

void GL_Graphic_Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0f);

	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	//glCullFace(GL_FRONT);

	/*
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	*/


	//glEnable(GL_CULL_FACE);

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, 100, 100, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void UI_Sleep(int time_ms)
{
	SDL_Delay(time_ms);
}

void UI_Clear()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UI_FlipBuffer()
{
	SDL_GL_SwapWindow(Window);
}

void UI_init()
{
	// Slightly different SDL initialization
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    GL_Graphic_Init();
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

}

void UI_Window(const char *title,int w,int h)
{
	Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

    if (Window == NULL)
    {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}

	if(glewInit() != GLEW_OK)
	{
		printf("Could not init glew.\n");
	}

}


/*

SDL_GL_DeleteContext(context);
SDL_DestroyWindow(window);
SDL_Quit();
*/

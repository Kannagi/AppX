#include <stdlib.h>
#include <stdio.h>


#include <SDL2/SDL.h>

#include "AppX.h"

void Event_Init(Event &event)
{
    for(int i = 0;i < EVENT_MAX;i++)
        event.key[i] = 0;

    event.exit = 0;
}

void Event_Update(Event *event)
{
	SDL_Event sdlevent;
	unsigned int i;

	event->clikright = 0;
	event->clikleft = 0;

	event->exit = 0;

	for(i = 0;i < EVENT_MAX;i++)
	{
		if(event->key[i] == 1)
			event->key[i] = 2;
		else
		if(event->key[i] == 3)
			event->key[i] = 0;
	}


	while(SDL_PollEvent(&sdlevent) == 1)
	{

		switch(sdlevent.type)
		{
			case SDL_MOUSEMOTION :
				event->mousex = sdlevent.motion.x;
				event->mousey = sdlevent.motion.y;
			break;

			case SDL_QUIT:
				event->exit = 1;
			break;

			case SDL_MOUSEBUTTONDOWN:
				switch(sdlevent.button.button)
				{
					case SDL_BUTTON_RIGHT:
						event->clikright = 1;
					break;

					case SDL_BUTTON_LEFT:
						event->clikleft = 1;
					break;
				}
			break;

			case SDL_MOUSEBUTTONUP:
				switch(sdlevent.button.button)
				{
					case SDL_BUTTON_RIGHT:
						event->clikright = 2;
					break;

					case SDL_BUTTON_LEFT:
						event->clikleft = 2;
					break;
				}
			break;

			case SDL_KEYDOWN:
				i = sdlevent.key.keysym.sym & 0xFFF;
				event->key[i] = 1;
			break;

			case SDL_KEYUP:
				i = sdlevent.key.keysym.sym & 0xFFF;
				event->key[i] = 3;
			break;
			/*
			case SDL_JOYBUTTONDOWN:

			for(i=0;i<20;i++)
			{
			if (sdlevent.jbutton.button == i)
			{
			event->touche[MNSJ + i] = 1;
			event->touche_ext[MNSJ + i] = 1;
			}
			}

			break;

			case SDL_JOYBUTTONUP:

			for(i=0;i<20;i++)
			{
			if (sdlevent.jbutton.button == i)
			{
			event->touche[MNSJ + i] = 2;
			event->touche_ext[MNSJ + i] = 0;
			}
			}

			break;

			case SDL_JOYAXISMOTION:


			if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value < -20000)
			{
			event->touche[MNSJ_LEFT] = 1;
			event->touche_ext[MNSJ_LEFT] = 1;
			}

			if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value >= -20000)
			{
			event->touche[MNSJ_LEFT] = 2;
			event->touche_ext[MNSJ_LEFT] = 0;
			}

			if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value > 20000)
			{
			event->touche[MNSJ_RIGHT] = 1;
			event->touche_ext[MNSJ_RIGHT] = 1;
			}

			if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value <= 20000)
			{
			event->touche[MNSJ_RIGHT] = 2;
			event->touche_ext[MNSJ_RIGHT] = 0;
			}

			if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value < -20000)
			{
			event->touche[MNSJ_UP] = 1;
			event->touche_ext[MNSJ_UP] = 1;
			}

			if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value >= -20000)
			{
			event->touche[MNSJ_UP] = 2;
			event->touche_ext[MNSJ_UP] = 0;
			}

			if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value > 20000)
			{
			event->touche[MNSJ_DOWN] = 1;
			event->touche_ext[MNSJ_DOWN] = 1;
			}

			if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value <= 20000)
			{
			event->touche[MNSJ_DOWN] = 2;
			event->touche_ext[MNSJ_DOWN] = 0;
			}


			*/
			break;
			default:
			break;
		}
	}

}

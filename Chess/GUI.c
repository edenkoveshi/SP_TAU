/*
 * GUI.c
 *
 *  Created on: 11 Aug 2017
 *      Author: amir
 */
#include "GUI.h"

int playGUI(){
	CHWindow* window = NULL;
	CHState* state = NULL;
	SDL_Event event;
	SP_BUFF_SET()
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
			printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
			return 0;
		}
		//graphical data
		window = createMainWindow();
		//logical data
		state = initialCHState();
		if (window == NULL || state == NULL ) {
			//both NULL safe
			destroyWindow(window);
			destroyCHState(state);
			SDL_Quit();
			return 0;
		}
		window->drawWindow(window,state);

		while (1) {
			SDL_WaitEvent(&event);
			if (!window->handleEventWindow(window,&event,state) || state->screen == SCREEN_QUIT)
				break;
			if (state->redraw && !(window -> drawWindow(window,state)))
					break;
		}
		//destroys all widgets window
		destroyWindow(window);
		//destroys logical data
		destroyCHState(state);
		SDL_Quit();
		return 1;
}

#ifndef CHWINDOW_H_
#define CHWINDOW_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include "CHGuiState.h"

/*
 * generic struct for window
 */
typedef struct chwindow_t CHWindow;
struct chwindow_t {
	void* data;
	bool (*drawWindow)(CHWindow*, CHState*);
	bool (*handleEventWindow)(CHWindow* , SDL_Event* ,CHState*);
	void (*destroyWindow)(CHWindow* );
};

//NULL safe FUNCTION
void destroyWindow(CHWindow* src);

#endif

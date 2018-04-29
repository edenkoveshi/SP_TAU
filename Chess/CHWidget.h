#ifndef CHWIDGET_H_
#define CHWIDGET_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdlib.h>
#include "CHGuiState.h"

/*
 * generic struct for widget
 */
typedef struct widget_t {
	bool (*drawWidget)(struct widget_t*);
	bool (*updateAppearance)(struct widget_t*,CHState*);
	void (*handleEvent)(struct widget_t*, SDL_Event*,CHState*);
	void (*destroyWidget)(struct widget_t*);
	void* data;
} CHWidget;

//This function would be usefull for NULL safe desetroy
void destroyWidget(CHWidget* src);
#endif

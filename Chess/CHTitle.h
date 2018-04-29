#ifndef CHTITLE_H_
#define CHTITLE_H_
#include <stdlib.h>
#include <stdio.h>
#include "CHCommon.h"
#include "CHWidget.h"
#include <stdbool.h>

/*
 * CHTitle represents a static CHWidget
 */
typedef struct ch_title_t CHTitle;
struct ch_title_t{
	SDL_Texture* titleTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
};

/**
 * Constructor for CHWidget with CHTitle data
 * location and sizes of the title depends on the given SDL_Rect
 * Appearance depends on bmp image found at the given 'image' path
 *
 * @return
 * a new CHWidget with CHTitle data
 * or NULL on error.
 *
 */
CHWidget* createTitle(SDL_Renderer* , SDL_Rect* ,const char* image);

/*
 * CHTitle function for CHWidget generic function "destroyWidget"
 * destroy all memory allocations of CHTitle
 * this function is NULL safe
 */
void destroyTitle(CHWidget*);

/**
 * CHTitle function for CHWidget generic function "handleEvent"
 * this is a no-op function.
 */
void handleTitleEvenet(CHWidget* , SDL_Event* , CHState*);

/**
 * CHTitle function for CHWidget generic function "drawWidget"
 * this is a no-op function.
 * @return
 * true
 */
bool drawTitle(CHWidget* );

/**
 * CHTitle function for CHWidget generic function "updateAppearance"
 * this is a no-op function.
 * @return
 * true
 */
bool updateTitleAppearance(CHWidget*,CHState*);

#endif

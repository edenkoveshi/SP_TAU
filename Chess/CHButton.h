#ifndef CHBUTTON_H_
#define CHBUTTON_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "CHCommon.h"
#include "CHWidget.h"
#include "GlobalDef.h"

/*
 * represents all types of button
 */
typedef enum button_type {
	BT_NEWGAME,
	BT_START,
	BT_EXIT,
	BT_NEXT,
	BT_LOAD,
	BT_GAMESLOT,
	BT_BACK,
	BT_ONEPLAYER,
	BT_TWOPLAYERS,
	BT_NOOB,
	BT_EASY,
	BT_MODERATE,
	BT_HARD,
	BT_EXPERT,
	BT_BLACK,
	BT_WHITE,
	BT_RESTART,
	BT_SAVE,
	BT_UNDO,
	BT_MAINMENU,

} BUTTON_TYPE;

/*
 * CHButton represents a clickable button from one of the BUTTON_TYPE
 * The button may be active/ disabled
 */
typedef struct ch_button_t CHButton;
struct ch_button_t{
	SDL_Surface* buttonSurface;
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool isActive;
	BUTTON_TYPE buttonType;
};

/**
 * Constructor for CHWidget with CHButton data
 * location and sizes of the button depends on the given SDL_Rect
 * Appearance depends on bmp image found at the given 'image' path
 *
 * @return
 * a new CHWidget with CHButton data
 * or NULL on error.
 *
 */
CHWidget* createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,BUTTON_TYPE buttonType);

/*
 * CHButton function for CHWidget generic function "destroyWidget"
 * destroy all memory allocations of CHButton
 * this function is NULL safe
 */
void destroyButton(CHWidget*);

/**
 * CHButton function for CHWidget generic function "handleEvent"
 * The CHState is updated according to the event
 */
void handleButtonEvenet(CHWidget*, SDL_Event*,CHState*);

/**
 * CHButton function for CHWidget generic function "drawWidget"
 * redraw button according to its 'isActive' state:
 * Draw button with rectangle on its perimeter iff isActive==true
 *
 * @return
 * true on success, false otherwise
 */
bool drawButton(CHWidget*);

/**
 * CHButton function for CHWidget generic function "updateAppearance"
 * changes CHButton variable 'isActive' according to the given CHState.
 * @return
 * true on success, false otherwise
 */
bool updateButtonAppearance(CHWidget*,CHState*);

/**
 * changing the CHButton display image according to the given bmp 'image' path
 * @return
 * true on success, false otherwise
 */
bool replaceButton(CHButton* data,const char* image, BUTTON_TYPE buttonType);

/**
 * @return
 * true- if the event was left mouse click and it occurs on the given CHButton
 * false- otherwise
 */
bool buttonChosen(CHButton* data, SDL_Event* event);

#endif

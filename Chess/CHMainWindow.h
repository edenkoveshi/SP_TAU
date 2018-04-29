#ifndef CHMAINWINDOW_H_
#define CHMAINWINDOW_H_
#include "CHWindow.h"
#include "CHWidget.h"
#include "CHGuiState.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CHButton.h"
#include "CHPiece.h"
#include "CHTitle.h"
#include "GlobalDef.h"
#include <time.h>

typedef struct ch_mainWindow_t  CHMainWindow;
struct ch_mainWindow_t {
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int numOfWidgets;
	CHWidget** widgets; // hold current button, titles etc of the window
	CHWidget** gameWidgets; //hold the game pieces widgets
};


/**
 * Constructor for CHWindow with CHMainWindow data
 * window sizes are defined at globalDef.h
 *
 * @return
 * a new CHWindow with CHMainWindow data
 * or NULL on error.
 */
CHWindow* createMainWindow();

/*
 * CHMainWindow function for CHWindow generic function "destroyWindow"
 * destroy all memory allocations of CHMainWindow
 * this function is NULL safe
 */
void destroyMainWindow(CHWindow* src);

/**
 * Destroys all CHMainWindow Widgets
 * NULL safe
 */
void clearWidgets(CHMainWindow* data);

/**
 * CHMainWindow function for CHWindow generic function "drawWindow"
 * the window is redrawn according to the CHstate.
 *
 * @return
 * true on success, false otherwise
 */
bool drawMainWindow(CHWindow*, CHState*);

/**
 * CHMainWindow function for CHWindow generic function "handleEventWindow"
 * Call to handleEvent function for all its widgets
 * The CHState is updated according to the event
 *
 * @return
 * true on success, false otherwise
 */
bool handleEvenetMainWindow(CHWindow* src, SDL_Event* event, CHState*);

/**
 * Main function to fill CHMainWindow widgets when screen is changing.
 *
 * @return
 * CHWidget** according to CHState->screen
 */
CHWidget** getMainButtons(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_LOAD
 *
 * @return
 * CHWidget** for SCREEN_LOAD
 */
CHWidget** getLoadButtons(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_MAIN
 *
 * @return
 * CHWidget** for SCREEN_MAIN
 */
CHWidget** fillMainWindowWidgets(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_MODE
 *
 * @return
 * CHWidget** for SCREEN_MODE
 */
CHWidget** getGameModeButtons(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_DIFF
 *
 * @return
 * CHWidget** for SCREEN_DIFF
 */
CHWidget** getDiffButtons(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_USERCOL
 *
 * @return
 * CHWidget** for SCREEN_USERCOL
 */
CHWidget** getUsercolButtons(CHMainWindow* data, CHState* state);

/**
 * this function is called from getMainButtons
 * when screen is changed to SCREEN_PLAY
 *
 * @return
 * CHWidget** for SCREEN_PLAY
 */
CHWidget** getPlayButtons(CHMainWindow* data);

/**
 * this function is called when we need to redraw the game board
 *
 * @return
 * CHWidget** represent the current state->game->board
 */
CHWidget** getPlayPieces(CHMainWindow* data, CHState* state);

/**
 * Saves in state->bgSpaces and state->settingSpaces Random positions for buttons
 * according to state->screen
 */
void setBG(CHState* state);

/*
 * draws background for the different settings screens
 */
void drawBGsettings(CHMainWindow* data, CHState* state);

/*
 * draw background for the playing screen
 */
void drawBGgame(CHMainWindow* data);

/*
 * Drawing possible moves on board according to state->getMovesArr
 * Colors for different moves type are defined at globalDef.h
 */
void drawGetMoves(CHMainWindow* data, CHState* state);
/*
 * update gameWidgets according to the given Move
 * which done by the user/computer
 */
void updateLastMovement(Move move,CHMainWindow* data );

/**
 * if needed, makes a computer move on state->game
 * and save that move at state -> lastComputerMove
 *
 * @return
 * true on success, false otherwise
 */
bool addComputerMove(CHState* state);

/*
 * Declares TIE/ CHECK/ CHECKMATE by pop-up window
 * when needed
 */
void declareGameStatus(CHState* state);

#endif

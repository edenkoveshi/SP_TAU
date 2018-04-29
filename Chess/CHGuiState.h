/*
 * CHGuiState.h
 *
 *  Created on: 30 Aug 2017
 *      Author: amir
 */

#ifndef CHGUISTATE_H_
#define CHGUISTATE_H_
#include <stdbool.h>
#include <stdlib.h>
#include "Game.h"
#include "Move.h"

typedef enum screen_type {
	SCREEN_MAIN,
	SCREEN_MODE,
	SCREEN_DIFF,
	SCREEN_USERCOL,
	SCREEN_LOAD,
	SCREEN_PLAY,
	SCREEN_QUIT
} SCREEN_TYPE;

/*
 * This struct holds all GUI state variables
 * The main window and its widgets may change CHState according to different events.
 * The main window and its widgets are redrawn according to CHState.
 */
typedef struct ch_gui_state {
	// game state
	Game* game;
	//Changing screen variables
	SCREEN_TYPE screen;
	bool redraw;
	bool screenChanged;
	bool declareStatus;
	// settings screens variables
	int settingSpaces[8];
	bool bgSpaces[2][ROWS];
	// dragging chess piece variables
	bool onDrag;
	int dragX;
	int dragY;
	// Variables For updading CHPiece positions
	Move lastUserMove;
	Move lastComputerMove;
	// Variables For handle get_moves command
	bool onGetMoves;
	Move* getMovesArr;
} CHState;

/**
 * Constructor for CHState
 * All variables are filled with their natural starting values
 * @return
 * a new CHState
 *
 */
CHState* initialCHState();

/**
 * destroy all memory allocations of CHState
 * this function is NULL safe
 */
void destroyCHState(CHState*);

#endif /* CHGUISTATE_H_ */

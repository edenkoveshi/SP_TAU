/*
 * CHGuiState.c
 *
 *  Created on: 30 Aug 2017
 *      Author: amir
 */

#include "CHGuiState.h"
#include "GlobalDef.h"

CHState* initialCHState(){
	CHState* state = malloc(sizeof(CHState));
	Game* game = CreateGame(1,2,1);
	if (game == NULL || state ==NULL){
		// both NULL safe
		DestroyGame(game);
		destroyCHState(state);
		printEr("initialCHState");
		return NULL;
	}
	state -> dragX = state -> dragY = 0;
	state -> onDrag = false;
	state -> game = game;
	state -> lastUserMove = getEmptyMove();
	state -> lastComputerMove = getEmptyMove();
	state -> screen = SCREEN_MAIN; //first window is main menu
	state -> redraw = true; //need to redraw at start
	state -> screenChanged = true; //need to fill widgets
	state -> onGetMoves = false;
	state -> getMovesArr = NULL;
	state -> declareStatus =false;
	return state;
}

void destroyCHState(CHState* state){
	if(state != NULL){
		DestroyGame(state->game);
		destroyMove(state-> getMovesArr);
		free(state);
	}
}

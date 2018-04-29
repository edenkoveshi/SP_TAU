/*
 * SPFIARGame.c
 *
 *  Created on: 30 May 2017
 *      Author: amir
 */



#include <stdbool.h>
#include <stdio.h>
#include "SPArrayList.h"
#include "SPFIARGame.h"
#include <stdlib.h>



//Definitions

#define _board src->gameBoard
#define _tops src->tops
#define _currentPlayer src->currentPlayer
#define _history src->history

SPFiarGame* spFiarGameCreate(int historySize){
	SPFiarGame* game = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (game ==null) //memory allocation failed
	{
		printf ("Error: spFiarGameCreate has failed");
		return null;
	}
	for (int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){ //initialize an empty board
		game->tops[i] = 0;
		for (int j=0; j<SP_FIAR_GAME_N_ROWS; j++){
				game->gameBoard[j][i] = SP_FIAR_GAME_EMPTY_ENTRY;
			}
	}
	game->history = spArrayListCreate(historySize);
	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	game->level = 0; //will be updated according to user input
	if (historySize<= 0 || game->history == null) return null; //history creation failed
	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if (src == null) return null; //invalid parameter
	SPFiarGame* game = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (game ==null) //memory allocation failed
		{
			printf ("Error: spFiarGameCopy has failed");
			return null;
		}
		for (int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){ //copy board
			game->tops[i] = _tops[i];
			for (int j=0; j<SP_FIAR_GAME_N_ROWS; j++){
					game->gameBoard[j][i] = _board[j][i];
				}
		}
	game->history = spArrayListCopy(_history);
	game->currentPlayer = _currentPlayer;
	game->level = src->level;
	return game;
}


void spFiarGameDestroy(SPFiarGame* src){
	spArrayListDestroy(_history);
	free(src);
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if (src == null || col<0 || col > SP_FIAR_GAME_N_COLUMNS -1 || _tops[col] == SP_FIAR_GAME_N_ROWS) return false;
	return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if (src == null || col<0 || col > SP_FIAR_GAME_N_COLUMNS -1) return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (!spFiarGameIsValidMove(src,col)) return SP_FIAR_GAME_INVALID_MOVE;
	_board[_tops[col]][col] = _currentPlayer; //place player symbol on top of selected column
	_tops[col]++;
	if (_currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) _currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else _currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL; //pass turn
	if (spArrayListIsFull(_history)) spArrayListRemoveFirst(_history); //if history is full, make place for the new move
	spArrayListAddLast(_history,col); //save new move in game's history
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if (src == null) return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(_history)) return SP_FIAR_GAME_NO_HISTORY;
	int lastMove = spArrayListGetLast(_history);
	_board[_tops[lastMove]-1][lastMove] = SP_FIAR_GAME_EMPTY_ENTRY;
	_tops[lastMove]--;
	if (_currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL){
		_currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		printf("Remove disc: remove user\'s disc at column %d\n",lastMove+1);
	}
	else{
		_currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		printf("Remove disc: remove computer\'s disc at column %d\n",lastMove+1);
	}

	spArrayListRemoveLast(_history);
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src == null) return SP_FIAR_GAME_INVALID_ARGUMENT;

	for (int i = SP_FIAR_GAME_N_ROWS-1;i >=0 ; i--){
		printf("|"); //print borders
		for (int j = 0; j <SP_FIAR_GAME_N_COLUMNS; j++){
			printf(" %c",_board[i][j]); //print board entries

			}
		printf(" |\n");
		}
	for (int j = 0; j <2*SP_FIAR_GAME_N_COLUMNS + 3; j++){
		printf("-");
	}
	printf("\n ");
	for (int j = 1; j <= SP_FIAR_GAME_N_COLUMNS; j++){
			printf(" %d",j);
	}
	printf("  \n");
	return SP_FIAR_GAME_SUCCESS;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if (src == null) return SP_FIAR_GAME_EMPTY_ENTRY;
	return _currentPlayer;
}


char spFiarCheckWinner(SPFiarGame* src){
	if(_history->actualSize==0) return '\0';
	int lastCol = spArrayListGetLast(_history);
	int lastRow = _tops[lastCol]-1;
	char lastPlayer = _currentPlayer==SP_FIAR_GAME_PLAYER_1_SYMBOL ? SP_FIAR_GAME_PLAYER_2_SYMBOL : SP_FIAR_GAME_PLAYER_1_SYMBOL;
	// row check
	if (lastRow >= 3 && _board[lastRow-1][lastCol] == lastPlayer && _board[lastRow-2][lastCol] == lastPlayer && _board[lastRow-3][lastCol] == lastPlayer){
		return lastPlayer;
	}

	for (int i = 0; i<4; i++){
		// column check
		if (lastCol-i >=0 && lastCol-i + 3 < SP_FIAR_GAME_N_COLUMNS){
			if (_board[lastRow][lastCol-i] == lastPlayer && _board[lastRow][lastCol-i+1] == lastPlayer && _board[lastRow][lastCol-i+2] == lastPlayer && _board[lastRow][lastCol-i+3] == lastPlayer){
				return lastPlayer;
			}
		}
		// right diagonal check
		if (lastCol-i >=0 && lastCol-i + 3 < SP_FIAR_GAME_N_COLUMNS && lastRow-i >= 0 && lastRow-i+3<SP_FIAR_GAME_N_ROWS){
			if (_board[lastRow-i][lastCol-i] == lastPlayer && _board[lastRow-i+1][lastCol-i+1] == lastPlayer && _board[lastRow-i+2][lastCol-i+2] == lastPlayer && _board[lastRow-i+3][lastCol-i+3] == lastPlayer){
							return lastPlayer;
						}
		}
		// left diagonal check
		if (lastCol-i >=0 && lastCol-i + 3 < SP_FIAR_GAME_N_COLUMNS && lastRow+i < SP_FIAR_GAME_N_ROWS && lastRow+i-3 >= 0){
					if (_board[lastRow+i][lastCol-i] == lastPlayer && _board[lastRow+i-1][lastCol-i+1] == lastPlayer && _board[lastRow+i-2][lastCol-i+2] == lastPlayer && _board[lastRow+i-3][lastCol-i+3] == lastPlayer){
									return lastPlayer;
								}
				}
	}

	for (int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){
		if (_tops[i] < SP_FIAR_GAME_N_ROWS){
			return '\0';
		}
	}
	return SP_FIAR_GAME_TIE_SYMBOL;
}



/*
 * Console.h
 *
 *  Created on: 11 Aug 2017
 *      Author: amir
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "Game.h"
#include "Parser.h"
#include "Move.h"
#include "GlobalDef.h"
#include "MiniMax.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * initial a console-mode game
 */
void playConsole();

/**
 * turn to settings state
 * restarts the game
 */
Game* restartGame(Game*);

/**
 * main playing loop
 */
void playGame(Game*);

/**
 * make a computer turn using MINIMAX algorithm
 * returns true on success
 */
bool playComputerTurn(Game*);

/**
 * make a user turn according to user input
 * returns true on success
 */
bool playUserTurn(Game*);

/**
 * free memory and quits the game
 */
void quitGame(Game* );

/**
 * prints game settings
 */
void printSettings(Game*);

/**
 * prints undo message according to the given move
 */
void printUndoMove(Game*,Move);

/**
 * prints computer move message according to the given move and source piece
 */
void printComputerMove(Move ,Piece);
/**
 * printing the board as the specified format
 */
void printBoard(Game *game);

/**
 * printing the list of given moves for get_moves command
 * game - current game
 * row - origin row
 * col - origin column
 */
void printMoves(Game *game, int row , int col);
#endif /* CONSOLE_H_ */

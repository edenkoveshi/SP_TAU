#ifndef MINIMAX_H_
#define MINIMAX_H_
#include "Move.h"
#include "Game.h"
#include "GlobalDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given by settings difficulty. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @return
 * NULL if either currentGame is NULL or memory allocation failure.
 * On success the function returns a Move struct which contains the
 *  best move move parameters.
 */
Move getComputerMove(Game*);

/*
 * returns the best board score according to current game
 * and fills the best move.
 * @param Game* - current game
 * @param Move* - Move pointer to best move which has to be filled.
 * @param int - alpha value := minimum upper bound.
 * @param int - beta value := maximum lower bound.
 * @param int - current depth in tree
 * @return score or -infinity on memory allocation failure.
 */
int getBestMove(Game*,Move*, int, int, int);

 /*
  * A more complex evaluation function
  * used when playing on expert difficulty level
  */
int getExpertScore(Game* game);

#endif

/*
 * Move.h
 *
 *  Created on: 6 Aug 2017
 *      Author: amir
 */
#ifndef MOVE_H_
#define MOVE_H_
#include "GlobalDef.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	THR,//Threatened by opponent
	CAP, //Capture opponent
	THR_CAP, // Both
	REST, // Neither
	NULL_MOVE,
} MOVE_TYPE;

typedef struct move_struct {
	int source[2];
	int target[2];
	MOVE_TYPE type;
	Piece targetPiece;
}Move;

/**
 * free move list
 */
void destroyMove(Move*);

/**
 * copy data from Move b to Move a
 */
void moveCopy(Move* a, Move* b);

/**
 * get all 0 move
 */
Move getEmptyMove();

#endif

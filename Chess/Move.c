/*
 * Move.c
 *
 *  Created on: 6 Aug 2017
 *      Author: amir
 */

#include "Move.h"

void moveCopy(Move* a, Move* b){
	if (a!=NULL && b!=NULL){
		a->source[0] = b->source[0];
		a->source[1] = b->source[1];
		a->target[0] = b->target[0];
		a->target[1] = b->target[1];
		a->type = b->type;
		a->targetPiece = b->targetPiece;
	}
}
void destroyMove(Move* moveList){
	if (moveList!=NULL){
		free(moveList);
	}
}

Move getEmptyMove(){
	Move move = {{0,0},{0,0},NULL_MOVE,BLANK};
	return move;
}



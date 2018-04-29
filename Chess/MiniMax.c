#include "MiniMax.h"

Move getComputerMove(Game* game){
	Move bestMove = {{0,0},{0,0},REST,EMPTY};
	int check = getBestMove(game, &bestMove, -INT_MAX, INT_MAX, 0);
	if (check == -INT_MAX) return getEmptyMove();
	return bestMove;
}

int getBestMove(Game* game,Move* bestMove, int alpha, int beta, int depth){
	Move *moveList,*curMove;
	Game* gameTmp;
	int score;
	//end of tree
	if (STATUS == CHECKMATE){
		if (TURN == USERCOL)
			return (INT_MAX / 2);
		return -(INT_MAX/2);
	}
	else if (depth == game->diff || STATUS == TIE || (game->diff == 5 && depth == game->diff-1)){
		// Invariant: -(2*(depth%2)-1) *((TURN)*2-1) = 1 iff computer is white
		//            -(2*(depth%2)-1) *((TURN)*2-1) = -1 iff computer is black
		if (DIFF < 5)
			return -(game->score*(2*(depth%2)-1) *((TURN)*2-1));
		return -(getExpertScore(game)*(2*(depth%2)-1) *((TURN)*2-1));
	}
	//continue recursion
	else{
		for (int i=0; i<ROWS; ++i){
				for (int j=0; j<COLS; ++j){
					moveList = curMove = PossibleMoves(game,i,j);
					while (curMove!=NULL && curMove->type != NULL_MOVE){
						gameTmp = copyGame(game);
						if (gameTmp == NULL){
							printEr("spMiniMaxBestMove");
							return -INT_MAX;
						}
						if ( !MakeMove(gameTmp,*curMove)){
							printEr("spMiniMaxBestMove");
							DestroyGame(gameTmp);
							destroyMove(moveList);
							return -INT_MAX;
						}
						updateStatus(gameTmp);
						score = getBestMove(gameTmp, bestMove, alpha, beta, depth+1);
						DestroyGame(gameTmp);
						//max node
						if (depth%2 == 0 && score > alpha){
							alpha = score;
							//root
							if (depth==0) moveCopy(bestMove,curMove);
						}
						//min node
						else if (depth%2 == 1 && score < beta) beta = score;
						//Pruning
						if (alpha >= beta){
							destroyMove(moveList);
							if (depth%2 == 0) return alpha;
							return beta;
						}
						curMove++;
					}
					destroyMove(moveList);
				}
			}
		}
	if (depth%2 == 0) return alpha;
	return beta;
}

//Expert methods
void updateExpertValues(Move* moves,int* mobility, int* threatened, int color){
	int i = 0;
	if (moves == NULL)
		return;
	while (moves[i].type != NULL_MOVE){
		*mobility += 2*color -1;
		if (moves[i].type == CAP || moves[i].type == THR_CAP)
			*threatened += getPieceScore(moves[i].targetPiece);
		++i;
	}
}

int getExpertScore(Game* game){
	int i, j, color;
	int mobility = 0;
	int threatened = 0;
	int turn = TURN;
	Move *moves = NULL;
	for (i=0; i<ROWS; ++i){
		for (j=0; j<COLS; ++j){
			color = pieceColor(BOARD[i][j]);
			if (color == 2)
				continue;
			TURN = color;
			moves = PossibleMoves(game,i,j);
			updateExpertValues(moves, &mobility, &threatened , color);
			destroyMove(moves);
			TURN = turn;
		}
	}

	return (game->score + mobility/10 + threatened/10);

}




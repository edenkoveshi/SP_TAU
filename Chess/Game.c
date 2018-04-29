/*
 * Game.c
 *
 *  Created on: 4 Aug 2017
 *      Author: Eden
 */
#include "Game.h"

Game* CreateGame(int mode,int diff,int usercol)
{
	Game* game=(Game*) (malloc(sizeof(Game)));
	if(game==NULL){
		printEr("CreateGame");
		return NULL;
	}
	game->history = CHArrayListCreate(HISTORY_SIZE);
	if(HISTORY==NULL){
			printEr("CreateGame");
			DestroyGame(game);
			return NULL;
	}
	InitializeBoard(game);
	game->mode = (mode==-1) ? ONE_PLAYER : mode;
	game->diff= (diff==-1) ? 2 : diff;
	game->usercol=(usercol==-1) ? WHITE : usercol;
	return game;
}

void DestroyGame(Game* game)
{
	if (game!=NULL){
		CHArrayListDestroy(HISTORY);
		free (game);
	}
}

void InitializeBoard(Game* game)
{
	int i,j;
	game->bking_x=ROWS-1;
	game->bking_y=4;
	game->wking_x=0;
	game->wking_y=4;
	game->score=0;
	game->turn = WHITE;
	game->status = PLAYING;
	CHArrayListClear(HISTORY);
	for(j=0;j<COLS;j++) //fill rows 1,6 with pawns
	{
		game->board[1][j]='m';
		game->board[ROWS-2][j]='M';
	}

	game->board[0][0]='r'; //place rooks
	game->board[0][COLS-1]='r';
	game->board[ROWS-1][0]='R';
	game->board[ROWS-1][COLS-1]='R';

	game->board[0][1]='n';//place knights
	game->board[0][COLS-2]='n';
	game->board[ROWS-1][1]='N';
	game->board[ROWS-1][COLS-2]='N';

	game->board[0][2]='b';//place bishops
	game->board[0][COLS-3]='b';
	game->board[ROWS-1][2]='B';
	game->board[ROWS-1][COLS-3]='B';

	game->board[0][3]='q';//place queens
	game->board[ROWS-1][3]='Q';

	game->board[0][4]='k';//place kings
	game->board[ROWS-1][4]='K';

	for(i=2;i<ROWS-2;i++) //fill rest of the board with blanks
	{
		for(j=0;j<COLS;j++)
			game->board[i][j]=BLANK;
	}
}

bool copyGameData(Game* gamein, Game* gameout){
		int i,j;
		CHArrayListDestroy(gamein->history);
		gamein->history = CHArrayListCopy(gameout->history);
		if(gamein->history==NULL){
					printEr("copyGameData");
					return false;
		}
		gamein->bking_x=gameout->bking_x;
		gamein->bking_y=gameout->bking_y;
		gamein->wking_x=gameout->wking_x;
		gamein->wking_y=gameout->wking_y;
		gamein->turn=gameout->turn;
		gamein->score=gameout->score;
		gamein->mode=gameout->mode;
		gamein->usercol=gameout->usercol;
		gamein->diff=gameout->diff;
		for(i=0;i<ROWS;i++)
		{
			for(j=0;j<COLS;j++)
			{
				gamein->board[i][j]=gameout->board[i][j];
			}
		}
		return true;
}

Game* copyGame(Game* game){
	if(game==NULL) return NULL;//invalid input
	Game* copy = CreateGame(1,2,1);
	if(copy==NULL){
			printEr("copyGame");
			return NULL;
	}
	if (!copyGameData(copy,game)){
		DestroyGame(copy);
		return NULL;
	}
	return copy;
}

int pieceColor(Piece piece){
	if (piece==EMPTY) return 2;
	if(piece ==WPAWN || piece ==WBISHOP || piece ==WROOK || piece ==WKNIGHT || piece ==WKING ||piece ==WQUEEN)
		return WHITE;
	return BLACK;
}

bool SameColor(Piece x,Piece y)
{
	return (pieceColor(x) == pieceColor(y));
}
bool isValidPlace(int row,int col){
	return (row>=0 && row<ROWS && col>=0 && col<COLS);
}
bool isOpp(Game* game,int row,int col,int color,Piece p){
	int oppColor = isValidPlace(row,col)? pieceColor(BOARD[row][col]) : 2;
	return (oppColor!=2 && oppColor !=color && (((toupper(BOARD[row][col])) == (int)p)||tolower(BOARD[row][col])==(int)p));
}

Move* PossibleMoves(Game* game,int row,int col){
	Move *moves = NULL, move={{row,col},{row,col},REST,BLANK};
	Piece piece = BOARD[row][col];
	int color = pieceColor(piece);
	if (color != TURN) return moves;
	switch(piece){
		case BPAWN:
		case WPAWN:
			moves = fillPawnMoves(game,move,color);
			break;
		case WBISHOP:
		case BBISHOP:
			moves = fillBishopMoves(game,move);
			break;
		case WROOK:
		case BROOK:
			moves = fillRookMoves(game,move);
			break;
		case WKNIGHT:
		case BKNIGHT:
			moves = fillKnightMoves(game,move);
			break;
		case WQUEEN:
		case BQUEEN:
			moves = fillQueenMoves(game,move);
			break;
		case WKING:
		case BKING:
			moves = fillKingMoves(game,move);
			break;
		default:;
		}
	return moves;
}

Move* fillPawnMoves(Game* game,Move move, int color){
	int k = 0;
	Move* moves = (Move*) malloc(5*sizeof(Move));
	move.target[0] += color*2-1;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] += color*2-1;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -= color*2-1;
	move.target[1] -= 1;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] += 2;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.type = NULL_MOVE;
	while (k<5){
	moves[k++] = move;
	}
	return moves;
}

Move* fillRookMoves(Game* game,Move move){
	int i,k = 0;
	Move* moves = (Move*) malloc(15*sizeof(Move));
	// check vertical
	for (i = 0; i < ROWS; i++){
		if (i == move.source[0]) continue;
		move.target[0] = i;
		if (isValidMove(game, &move))
				moves[k++] = move;
	}
	move.target[0] = move.source[0];
	// check horizontal
	for (i = 0; i < COLS; i++){
		if (i == move.source[1]) continue;
		move.target[1] = i;
		if (isValidMove(game, &move))
				moves[k++] = move;
	}
	move.type = NULL_MOVE;
	while (k<15){
	moves[k++] = move;
	}
	return moves;

}

Move* fillBishopMoves(Game* game,Move move){
	int i,k = 0;
	Move* moves = (Move*) malloc(15*sizeof(Move));
	// check diagonal up right
	i = (move.source[0] < move.source[1]) ? 0 : (move.source[0]- move.source[1]);
	for ( ; i < ROWS && i+move.source[1]-move.source[0]<COLS; i++){
		if (i == move.source[0]) continue;
		move.target[0] = i;
		move.target[1] = i+move.source[1]-move.source[0];
		if (isValidMove(game, &move))
				moves[k++] = move;
	}
	// check diagonal bottom right
	i = (move.source[0] < COLS-1-move.source[1]) ? (move.source[0] + move.source[1]) : ROWS-1;
	for ( ; i >= 0 && move.source[0]+move.source[1]-i<COLS; i--){
			if (i == move.source[0]) continue;
			move.target[0] = i;
			move.target[1] = move.source[0] + move.source[1]-i;
			if (isValidMove(game, &move))
					moves[k++] = move;
		}
	move.type = NULL_MOVE;
	while (k<15){
	moves[k++] = move;
	}
	return moves;
}

Move* fillKnightMoves(Game* game,Move move){
	int k = 0;
	Move* moves = (Move*) malloc(9*sizeof(Move));

	move.target[0] +=1;	//up 1 right 2
	move.target[1] +=2;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] -=4;	//up 1 left 2
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -=2;	//down 1 left 2
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] +=4;	//down 1 right 2
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -=1;	//down 2 right 1
	move.target[1] -=1;
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] +=4;	//up 2 right 1
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] -=2;	//up 2 left 1
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -=4;	//down 2 left 1
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.type = NULL_MOVE;
	while (k<9){
	moves[k++] = move;
	}
	return moves;
}

Move* fillQueenMoves(Game* game,Move move){
	int i = 0,k = 0;
	Move *moves,*movesR,*movesB;
	moves = (Move*) malloc(29*sizeof(Move));
	movesR = fillRookMoves(game,move);
	move.type = REST;
	movesB = fillBishopMoves(game,move);
	while (movesR[i].type !=NULL_MOVE){
		moves[k++] = movesR[i++];
	}

	i = 0;
	while (movesB[i].type !=NULL_MOVE){
			moves[k++] = movesB[i++];
	}
	move.type = NULL_MOVE;
	while (k<29){
	moves[k++] = move;
	}
	destroyMove(movesR);
	destroyMove(movesB);
	return moves;
}

Move* fillKingMoves(Game* game,Move move){
	int k = 0;
	Move* moves = (Move*) malloc(9*sizeof(Move));
	move.target[0] +=1; //1 forward
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] +=1; //1 forward and right
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -=2; //1 backward and right
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] +=1; 
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] -=2; // 1 left
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] +=1; //1 forward and left
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[0] -=2; //1 backward and left
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.target[1] +=1; //1 backward
	if (isValidPlace(move.target[0],move.target[1]) && isValidMove(game, &move)){
		moves[k++] = move;
	}
	move.type = NULL_MOVE;
	while (k<9){
		moves[k++] = move;
	}
	return moves;
}

// if true, the function will update move type and target piece
bool isValidMove(Game* game, Move* moveBase){
	Move move = *moveBase;
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	Piece s=BOARD[row_src][col_src];
	Piece t=BOARD[row_tar][col_tar];
	bool isValid=false;
	//checks if source place contains piece in the current turn colour and target place is either empty or has opponent piece
	if(s==BLANK || pieceColor(s)!=game->turn || pieceColor(s) == pieceColor(t)) return isValid;
	switch(s){
	case WPAWN:
		isValid = PawnValidMove(move, game, WHITE);
		break;
	case BPAWN:
		isValid = PawnValidMove(move, game, BLACK);
		break;
	case WBISHOP:
		isValid = BishopValidMove(move,game,WHITE);
		break;
	case BBISHOP:
		isValid = BishopValidMove(move,game,BLACK);
		break;
	case WROOK:
		isValid = RookValidMove(move,game,WHITE);
		break;
	case BROOK:
		isValid = RookValidMove(move,game,BLACK);
		break;
	case WKNIGHT:
		isValid = KnightValidMove(move,game,WHITE);
		break;
	case BKNIGHT:
		isValid = KnightValidMove(move,game,BLACK);
		break;
	case WQUEEN:
		isValid = BishopValidMove(move,game,WHITE) || RookValidMove(move,game,WHITE);
		break;
	case BQUEEN:
		isValid = BishopValidMove(move,game,BLACK) || RookValidMove(move,game,BLACK);
		break;
	case WKING:
		isValid = KingValidMove(move,game,WHITE);
		break;
	case BKING:
		isValid = KingValidMove(move,game,BLACK);
		break;
	default:;
	}
	if (isValid){
		Game* copy = copyGame(game);
		MakeMove(copy,move);
		moveBase->targetPiece = t;
		moveBase->type = REST;
		if (isThreatened(copy,move.target[0],move.target[1]))
			moveBase->type = THR;
		if (t != BLANK){
			if (moveBase->type == THR){
				moveBase->type = THR_CAP;
			}
			else moveBase->type = CAP;
		}
		switch (copy->turn){
		case (BLACK):
			if (isThreatened(copy,copy->wking_x,copy->wking_y)) isValid = false;
			break;
		default:
			if (isThreatened(copy,copy->bking_x,copy->bking_y)) isValid = false;
		}
		DestroyGame(copy);
	}
	return isValid;
}

bool PawnValidMove(Move move, Game* game, int piececol){
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	int color = (pieceColor(BOARD[row_src][col_src])*2-1); // := 1 for white, -1 for black
	if((color==1&&piececol==0)||(color==-1&&piececol==1)) return false;
	if (BOARD[row_tar][col_tar]==BLANK && col_tar==col_src){  //same column
					if ( row_tar == row_src+color) //regular move
						return true;
					if (row_tar == row_src+2*color && row_src == 6-5*pieceColor(BOARD[row_src][col_src]) && BOARD[row_src+color][col_tar]==BLANK) //double move
						return true;
				}
	else if( BOARD[row_tar][col_tar]!=BLANK && (col_tar==col_src+1 || col_tar==col_src-1) && row_tar == row_src+color){ //eating move
		return true;
	}
		return false;
}

bool RookValidMove(Move move,Game* game,int piececol){
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	int color=pieceColor(game->board[row_src][col_src]);//1 for white,0 for black
	if(color!=piececol) return false;
	int dir = 1;
	if (row_src == row_tar){
		dir = col_tar > col_src ? 1 : -1;
		for (int i = col_src+dir ; i != col_tar ; i += dir){
			if (BOARD[row_src][i] != BLANK) return false;
		}
		return true;
	}
	else if (col_src == col_tar){
		dir = row_tar > row_src ? 1 : -1;
		for (int i = row_src+dir ; i != row_tar ; i += dir){
			if (BOARD[i][col_src] != BLANK) return false;
		}
		return true;
	}
	return false;
}

bool BishopValidMove(Move move,Game* game,int piececol){
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	int color=pieceColor(game->board[row_src][col_src]);//1 for white,0 for black
	if(color!=piececol) return false;
	int dirX = row_tar > row_src ? 1 : -1;
	int dirY = col_tar > col_src ? 1 : -1;
	if ((row_tar-row_src) * (row_tar-row_src) != (col_tar-col_src) * (col_tar-col_src)) return false;
	for (int i = row_src+dirX, j = col_src+dirY; i != row_tar ; i += dirX,j += dirY){
				if (BOARD[i][j] != BLANK) return false;
	}
	return true;
}

bool KnightValidMove(Move move,Game* game,int piececol){
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	int color=pieceColor(game->board[row_src][col_src]);//1 for white,0 for black
	if(color!=piececol) return false;
	if ((col_tar-col_src) * (col_tar-col_src) == 1 && (row_tar-row_src) * (row_tar-row_src) == 4) return true;
	if ((col_tar-col_src) * (col_tar-col_src) == 4 && (row_tar-row_src) * (row_tar-row_src) == 1) return true;
	return false;
}

bool KingValidMove(Move move,Game* game,int piececol){
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	int color=pieceColor(game->board[row_src][col_src]);//1 for white,0 for black
	if(color!=piececol) return false;
	//if (row_src == row_tar && (col_tar-col_src) * (col_tar-col_src) ==1) return true;//1 right or left
	//if (col_src == col_tar && (row_tar-row_src) * (row_tar-row_src) ==1) return true; //1 forward or backward
	if (((col_tar-col_src)*(col_tar-col_src)<=1) && ((row_tar-row_src)*(row_tar-row_src)<=1)&&
		((row_tar-row_src)*(row_tar-row_src)+(col_tar-col_src)*(col_tar-col_src)>0)) return true;
	return false;
}

// return true if piece on <row,col> is threatened by other opponent piece in the game
bool isThreatened(Game* game,int row,int col ){
	int color = pieceColor(BOARD[row][col]);
	if (color == 2) return false;
	return threatenedByPawn(game,row,col,color) || threatenedByRook(game,row,col,color) || threatenedByBishop(game,row,col,color)
			|| threatenedByKnight(game,row,col,color) || threatenedByKing(game,row,col,color);
}

bool threatenedByPawn(Game* game,int row_src,int col_src, int color){
	switch(color){
		case WHITE:
			if (isOpp(game, row_src+1,col_src+1,color,'M')) return true;
			if (isOpp(game, row_src+1,col_src-1,color,'M')) return true;
			break;
		case BLACK:
			if (isOpp(game, row_src-1,col_src+1,color,'m')) return true;
			if (isOpp(game, row_src-1,col_src-1,color,'m')) return true;
	}
	return false;
}

bool threatenedByKing(Game* game,int row_src,int col_src, int color){
	if (isOpp(game, row_src,col_src+1,color,'K')) return true; //right
	if (isOpp(game, row_src,col_src-1,color,'K')) return true; //left
	if (isOpp(game, row_src+1,col_src,color,'K')) return true; //up
	if (isOpp(game, row_src-1,col_src,color,'K')) return true; //down
	if (isOpp(game, row_src-1,col_src-1,color,'K')) return true; //down and left
	if (isOpp(game, row_src-1,col_src+1,color,'K')) return true; //down and right
	if (isOpp(game, row_src+1,col_src+1,color,'K')) return true; //up and right
	if (isOpp(game, row_src+1,col_src-1,color,'K')) return true; //up and left
	return false;
}

bool threatenedByKnight(Game* game,int row_src,int col_src, int color){
	if (isOpp(game, row_src+1,col_src-2,color,'N')) return true; //up 1 left 2
	if (isOpp(game, row_src+1,col_src+2,color,'N')) return true; //up 1 right 2
	if (isOpp(game, row_src-1,col_src-2,color,'N')) return true; //down 1 left 2
	if (isOpp(game, row_src-1,col_src+2,color,'N')) return true; //down 1 right 2
	if (isOpp(game, row_src+2,col_src+1,color,'N')) return true; //up 2 right 1
	if (isOpp(game, row_src-2,col_src+1,color,'N')) return true; //down 2 right 1
	if (isOpp(game, row_src+2,col_src-1,color,'N')) return true; //up 2 left 1
	if (isOpp(game, row_src-2,col_src-1,color,'N')) return true; //down 2 left 1
	return false;
}

bool threatenedByBishop(Game* game,int row_src,int col_src, int color){
	int i,oppColor;
	Piece oppP;
	// check up right
	for (i = 1; row_src+i< ROWS && col_src+i<COLS; i++){
		oppP = BOARD[row_src+i][col_src+i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'B' || toupper(oppP) == 'Q')){
				return true;}
			else break;
		}
	}
	// check up left
	for (i = 1; row_src+i< ROWS && col_src-i>=0; i++){
		oppP = BOARD[row_src+i][col_src-i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'B' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	// check down left
	for (i = 1; row_src-i >=0 && col_src-i>=0; i++){
		oppP = BOARD[row_src-i][col_src-i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'B' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	// check down right
	for (i = 1; row_src-i >=0 && col_src+i < COLS; i++){
		oppP = BOARD[row_src-i][col_src+i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'B' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	return false;
}

bool threatenedByRook(Game* game,int row_src,int col_src, int color){
	int i,oppColor;
	Piece oppP;
	// check up
	for (i = row_src+1; i< ROWS; i++){
		oppP = BOARD[i][col_src];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'R' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	// check down
	for (i = row_src-1; i >= 0; i--){
		oppP = BOARD[i][col_src];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'R' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	// check left
	for (i = col_src-1; i >= 0; i--){
		oppP = BOARD[row_src][i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'R' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	// check right
	for (i = col_src+1; i < COLS; i++){
		oppP = BOARD[row_src][i];
		oppColor = pieceColor(oppP);
		if (oppColor != 2){
			if ((oppColor != color) && (toupper(oppP) == 'R' || toupper(oppP) == 'Q'))
				return true;
			else break;
		}
	}
	return false;
}

bool hasValidMove(Game *game){
	int i,j;
	Move* moves;
	for ( i=0; i<ROWS; i++){
		for (j=0; j<COLS; j++){
			moves= PossibleMoves(game,i,j);
			if (moves!=NULL && moves[0].type!=NULL_MOVE) {
				destroyMove(moves);
				return true;
			}
			destroyMove(moves);
		}
	}
	return false;
}

//checks whether current player is checked. used for move validation
bool isCheck(Game* game){
	switch (game->turn){
	case (WHITE):
		return isThreatened(game,game->wking_x,game->wking_y);
	default:
		return isThreatened(game,game->bking_x,game->bking_y);
	}
}

bool MakeMove(Game* game, Move move)
{
	int row_src = move.source[0], col_src = move.source[1], row_tar = move.target[0], col_tar = move.target[1];
	//update history
	if (CHArrayListIsFull(HISTORY)) CHArrayListRemoveFirst(HISTORY); //if history is full, make place for the new move
	CHArrayListAddLast(HISTORY,move); //save new move in game's history
	//update king position
	if (BOARD[row_src][col_src] == WKING){
		game->wking_x = row_tar;
		game->wking_y = col_tar;
	}
	else if (BOARD[row_src][col_src] == BKING){
			game->bking_x = row_tar;
			game->bking_y = col_tar;
	}
	// make move
	BOARD[row_tar][col_tar] = BOARD[row_src][col_src];
	BOARD[row_src][col_src] = BLANK;
	TURN = (TURN==WHITE) ? BLACK : WHITE;
	SCORE-= getPieceScore(move.targetPiece);
	return true;
}
void updateStatus(Game* game){
	if (isCheck(game)){
		if (hasValidMove(game))
			STATUS = CHECK;
		else
			STATUS = CHECKMATE;
	}
	else if (hasValidMove(game))
		STATUS = PLAYING;
	else
		STATUS = TIE;
}


int getPieceScore(Piece piece){
	// white:= +1 black: -1 empty:0
	int color = 2*pieceColor(piece)-1, score = 0;
	switch (piece){
		case WPAWN:
		case BPAWN:
			score =  color;
			break;
		case WBISHOP:
		case BBISHOP:
			score = color*3;
			break;
		case WROOK:
		case BROOK:
			score = color*5;
			break;
		case WKNIGHT:
		case BKNIGHT:
			score = color*3;
			break;
		case WQUEEN:
		case BQUEEN:
			score = color*9;
			break;
		case WKING:
		case BKING:
			score = color*100;
			break;
		default:
			score=0;
			break;
	}
	return score;
}

Move Undo(Game* game)
{
	Move move = getEmptyMove();
	if (CHArrayListIsEmpty(HISTORY) || (CHArrayListSize(HISTORY)==1 && TURN == USERCOL))
		return move;
	move = CHArrayListGetLast(HISTORY);
	BOARD[move.source[0]][move.source[1]] = BOARD[move.target[0]][move.target[1]];
	BOARD[move.target[0]][move.target[1]] = move.targetPiece;

	if (BOARD[move.source[0]][move.source[1]] == WKING){
			game->wking_x = move.source[0];
			game->wking_y = move.source[1];
	}
	else if (BOARD[move.source[0]][move.source[1]] == BKING){
			game->bking_x = move.source[0];
			game->bking_y = move.source[1];
	}
	SCORE+=getPieceScore(move.targetPiece);
	CHArrayListRemoveLast(HISTORY);
	TURN = (TURN==WHITE) ? BLACK : WHITE;
	if (TURN == USERCOL)
		updateStatus(game);
	return move;
}

bool Save(Game* game,char* filepath)
{
	FILE* f=fopen(filepath,"w");
	char* str;
	int i,j;
	if (f==NULL)
	{
		return false;
	}
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n",f); //opening
	fputs("<game>\n",f);
	str=(char*) malloc(6000);
	sprintf(str,"\t<current_turn>%d<\\current_turn>\n",game->turn); //turn
	fputs(str,f);
	sprintf(str,"\t<game_mode>%d<\\game_mode>\n",game->mode); //mode
	fputs(str,f);
	if(game->mode==1)
	{
		sprintf(str,"\t<difficulty>%d<\\difficulty>\n",game->diff); //difficulty
		fputs(str,f);
		sprintf(str,"\t<user_color>%d<\\user_color>\n",game->usercol); //user color
		fputs(str,f);
	}
	fputs("\t<board>\n",f); //board print
	for(i=ROWS-1;i>=0;i--)
	{
		sprintf(str,"\t\t<row_%d>",i+1);
		fputs(str,f);
		for(j=0;j<COLS;j++)
		{
			fputc(game->board[i][j],f);
		}
		sprintf(str,"<\\row_%d>\n",i+1);
		fputs(str,f);
	}
	fputs("\t<\\board>\n",f); //end of board label
	fputs("<\\game>\n",f); //end of xml file
	fclose(f);
	free(str);
	return true;
}

Game* Load(char* filepath)
{
	FILE* f=fopen(filepath,"r");
	if(f==NULL)
	{
		return NULL;
	}
	char* str=malloc(100*sizeof(char));//100 chars is an upper bound for label length in the given format
	str=readLabel(f,str);//opening
	if(str==NULL) return NULL;
	str=readLabel(f,str);//game
	if(str==NULL) return NULL;
	int mode=1;
	int usercol=1;
	int diff=2;
	int turn=0;
	int i;
	str=readLabel(f,str);//current turn
	if(str==NULL) return NULL;
	i=parseLabel(str);
	turn=atoi(&str[i]);
	str=readLabel(f,str); //mode
	if(str==NULL) return NULL;
	i=parseLabel(str);
	mode=atoi(&str[i]);
	if(mode == 1)
	{
		str=readLabel(f,str); //difficulty
		if(str==NULL) return NULL;
		i=parseLabel(str);
		diff=atoi(&str[i]);
		str=readLabel(f,str);//user color
		if(str==NULL) return NULL;
		i=parseLabel(str);
		usercol=atoi(&str[i]);
	}
	Game* game=CreateGame(mode,diff,usercol);
	if(game==NULL)
		return NULL;
	game->turn=turn;
	str=readLabel(f,str); //can be either board or difficulty or user color
	if(mode!=1)
	{
		if(str[2]=='d' || str[2]=='u')
			str=readLabel(f,str);
		if(str[2]=='d' || str[2]=='u')
			str=readLabel(f,str);
	}
	if(str==NULL) return NULL;
	for(int j=ROWS-1;j>=0;j--)
	{
		str=readLabel(f,str);
		if(str==NULL) return NULL;
		i=parseLabel(str);
		for(int k=0;k<COLS;k++)
		{
			game->board[j][k]=str[i+k];
			if(str[i+k]=='K')
			{
				game->bking_x=j;
				game->bking_y=k;
			}
			if(str[i+k]=='k')
			{
				game->wking_x=j;
				game->wking_y=k;
			}
			game->score+=getPieceScore(game->board[j][k]);
		}
	}
	updateStatus(game);
	free(str);
	fclose(f);
	return game;
}

char* readLabel(FILE* f,char* str)
{
	int i=0;
	char ch=fgetc(f);
	while(ch!='\n' && ch!=EOF)
	{
		*(str+i)=ch;
		i++;
		ch=fgetc(f);
	}
	return str;
}

int parseLabel(char* label)
{
	int i=0;
	while(*(label+i)!='>')
		i++;
	return i+1;
}

Move* sortedMoves(Game* game,int row,int col){
	int n_moves;
	Move* moves = PossibleMoves(game,row,col);
	switch(BOARD[row][col]){
		case BPAWN:
		case WPAWN:
			n_moves = 5;
			break;
		case WBISHOP:
		case BBISHOP:
			n_moves = 15;
			break;
		case WROOK:
		case BROOK:
			n_moves = 15;
			break;
		case WKNIGHT:
		case BKNIGHT:
			n_moves = 9;
			break;
		case WQUEEN:
		case BQUEEN:
			n_moves = 29;
			break;
		case WKING:
		case BKING:
			n_moves = 9;
			break;
		default:
			n_moves = 0;
	}
	if (moves !=NULL)
		qsort(moves,n_moves,sizeof(Move),compare_Move);
	return moves;
}
int compare_Move(const void *a, const void  *b){
	const Move *m1 = a, *m2 = b;
	if (m1->type==NULL_MOVE) return 1;
	if (m2->type==NULL_MOVE) return -1;
	if ((m1->target[0] - m2->target[0]) != 0) return (m1->target[0] - m2->target[0]);
	return (m1->target[1] - m2->target[1]);
}

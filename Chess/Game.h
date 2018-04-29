/*
 * Game.h
 *
 *  Created on: 4 Aug 2017
 *      Author: Eden
 */

#ifndef GAME_H_
#define GAME_H_

#include "GlobalDef.h"
#include "Move.h"
#include "CHArrayList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum GAME_STATE
{
	PLAYING,
	TIE,
	CHECK,
	CHECKMATE,
} Status;

typedef struct game_t{
	char board[ROWS][COLS]; //game board
	int mode; //game mode
	int diff; //game difficulty
	int usercol; //user color
	int turn; // current turn, 0/1 represent white/black turn
	CHArrayList* history; // holds previous 7 moves, used for undo implementation
	int wking_x;//location of white king
	int wking_y;
	int bking_x;//location on black king
	int bking_y;
	int score; //score of board - assist for MinimMax
	Status status;
} Game;

/**
 * CreateGame(int mode,int diff,int usercol)
 * initializes a game according to given settings, sets default settings if settings were not chosen (-1)
 **/
Game* CreateGame(int mode,int diff,int usercol);

/**
 * initalizes game board
 */
void InitializeBoard(Game*);

/**
 * returns a copy of a given game
 */
Game* copyGame(Game* game);

/**
 * copy all game data from gameout to gamein
 * return true on success
 */
bool copyGameData(Game* gamein, Game* gameout);

/**
 * returns 1 if piece is white,0 otherwise
 */
int pieceColor(Piece piece);

/**
 * returns true if the two pieces belong to the same player (have the same color)
 */
bool SameColor(Piece x,Piece y);

/**
 * returns true if desired place is in board bounds, 0 otherwise
 */
bool isValidPlace(int row,int col);

/**
 * returns a pointer to an array containing all possible moves from position (row,col)
 */
Move* PossibleMoves(Game* game,int row,int col);

/**
 * given that a piece is pawn type, returns all it's possible moves
 */
Move* fillPawnMoves(Game* game,Move move,int color);

/**
 * given that a piece is rook type, returns all it's possible moves
 */
Move* fillRookMoves(Game* game,Move move);

/**
 * given that a piece is bishop type, returns all it's possible moves
 */
Move* fillBishopMoves(Game* game,Move move);

/**
 * given that a piece is knight type, returns all it's possible moves
 */
Move* fillKnightMoves(Game* game,Move move);

/**
 * given that a piece is queen type, returns all it's possible moves
 */
Move* fillQueenMoves(Game* game,Move move);

/**
 * given that a piece is king type, returns all it's possible moves
 */
Move* fillKingMoves(Game* game,Move move);


/**
 * returns true iff move is valid
 */
bool isValidMove(Game* game,Move* moveBase);

/**
 * returns true iff move is valid, given that a pawn is being moved
 */
bool PawnValidMove(Move move,Game* game, int piececol);

/**
 * returns true iff move is valid, given that a rook is being moved
 */
bool RookValidMove(Move move,Game* game,int piececol);

/**
 * returns true iff move is valid, given that a bishop is being moved
 */
bool BishopValidMove(Move move,Game* game,int piececol);

/**
 * returns true iff move is valid, given that a knight is being moved
 */
bool KnightValidMove(Move move,Game* game,int piececol);

/**
 * returns true iff move is valid, given that a king is being moved
 */
bool KingValidMove(Move move,Game* game,int piececol);

/**
 * return true if piece on <row,col> is threatened by other opponent piece in the game
 */
bool isThreatened(Game* game,int,int);

/**
 * return true if piece on <row_src,col_src> is threatened by other opponent's pawn
 */
bool threatenedByPawn(Game*,int,int,int);

/**
 * return true if piece on <row_src,col_src> is threatened by other opponent's rook
 */
bool threatenedByRook(Game*,int,int,int);

/**
 * return true if piece on <row_src,col_src> is threatened by other opponent's knight
 */
bool threatenedByKnight(Game*,int,int,int);

/**
 * return true if piece on <row_src,col_src> is threatened by other opponent's bishop
 */
bool threatenedByBishop(Game*,int,int,int);

/**
 * return true if piece on <row_src,col_src> is threatened by other opponent's king
 */
bool threatenedByKing(Game*,int,int,int);

/**
 * returns true iff player has valid move to make
 */
bool hasValidMove(Game *game);

/**
 * returns true iff player is checked
 */
bool isCheck(Game* game);

/**
 * makes a move from (x1,y1) to (x2,y2), returns 1 on success
 */
bool MakeMove(Game*,Move);

/**
 *destroys a game
 */
void DestroyGame(Game* game);

/**
 * undo
 */
Move Undo(Game* game);

/**
 * return piece value according to the naive scoring function
 */
int getPieceScore(Piece);
/*
 * update game status to either:
 * PLAYING, CHECK, TIE ,CHECKMATE
 */
void updateStatus(Game*);

/**
 * saves current game
 */
bool Save(Game* game,char* filepath);

/**
 * loads a game from given address. assumes that the address points to a file in the xml format
 */
Game* Load(char* filepath);

/**
 * reads a line from a file given in the xml format and writes it into str,returns str on success and NULL on failure
 */
char* readLabel(FILE* f,char* str);

/**
 * given a label, returns the position of the parameter in it
 */
int parseLabel(char* label);


/*
 * return all moves possible from <row,col>
 * the list is sorted ad specified in the project description
 */
Move* sortedMoves(Game* game,int row,int col);


/*
 * comparator for get_moves
 * return positive number if (Move*) a > (Move*) b or b is NULL_MOVE
 * return negative number if (Move*) a < (Move*) b or a is NULL_MOVE
 * else return don't care
 */
int compare_Move(const void *a, const void  *b);

#endif /* GAME_H_ */

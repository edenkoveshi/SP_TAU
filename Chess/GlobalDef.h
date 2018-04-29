/*
 * GlobalDef.h
 *
 *  Created on: 6 Aug 2017
 *      Author: amir
 */
#ifndef GLOBALDEF_H_
#define GLOBALDEF_H_


//backend defs
#define BLACK 0
#define WHITE 1
#define ONE_PLAYER 1
#define TWO_PLAYERS 2
#define ROWS 8
#define COLS 8
#define BLANK '_'
#define BOARD game->board
#define MODE game->mode
#define USERCOL game->usercol
#define DIFF game->diff
#define TURN game->turn
#define STATUS game->status
#define HISTORY game->history
#define SCORE game->score
#define MAX_LEN 6000
#define HISTORY_SIZE 7
#define printEr(str) printf("Error: %s has failed\n",str)

//frontend defs
#define WIN_H 600
#define OFFSET (WIN_H/250)
#define GRID_SIZE ((WIN_H-2*OFFSET)/ROWS)
#define BUTTON_W (GRID_SIZE*2-2*OFFSET)
#define BUTTON_H (GRID_SIZE-2*OFFSET)
#define PIECE_H (GRID_SIZE-2*OFFSET)
#define PIECE_W (GRID_SIZE-2*OFFSET)
#define WIN_W (GRID_SIZE*(ROWS)+BUTTON_W+4*OFFSET)
//#define SQ_WHITE 238,215,209,255 //pink
//#define SQ_BLACK 181,221,228,255 //blue
#define SQ_WHITE 222,191,129,255 //white
#define SQ_BLACK 153,76,0,255 //black
#define BG_COLOR 0,163,133,255 //green
#define PIECES_COLOR 255,221,0 //orange
//#define PIECES_COLOR 241,155,41 //orange
#define BUTTONS_COLOR 255,221,0 //yellow
#define MASK_COLOR 255,0,255 //magenta
//get_moves colors
#define THR_POSITION 159,48,29,125 // red
#define REST_POSITION 1,116,199,125 // blue
#define CAP_POSITION 0,163,133,125 // green
#define MAX_SLOTS 5
#define BUG_FIX 1
#if BUG_FIX
#define SP_BUFF_SET() {	               \
	setvbuf(stdout,NULL,_IONBF,0); 	   \
	setvbuf(stderr,NULL,_IONBF,0);     \
	setvbuf(stdin,NULL,_IONBF,0);      \
}
#else
#define SP_BUFF_SET(){}
#endif

typedef enum piece
{
	BPAWN='M',
	WPAWN='m',
	BBISHOP='B',
	WBISHOP='b',
	BROOK='R',
	WROOK='r',
	BKNIGHT='N',
	WKNIGHT='n',
	BQUEEN='Q',
	WQUEEN='q',
	BKING='K',
	WKING='k',
	EMPTY='_'
} Piece;

#endif


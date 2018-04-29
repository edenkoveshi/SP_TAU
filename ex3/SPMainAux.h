#include <stdio.h>
#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#define HISTORY_SIZE 20
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

/**
 * startGame()
 * Starts a new SPFIAR game
 */
void startGame();

/*
 * quit()
 * Quits the game
 * @param game - the current game that is played
 */
void quit(SPFiarGame* game);

/*
 * endingGame()
 * Handles the last play in the game.
 * @param game - the current game that is played
 */
void endingGame(SPFiarGame* game);

/*
 * getUserCommand()
 * Handles communication with the user.
 * @params
 * game - current gameplay
 * isEnded - true iff game is already over, enables user to use only 'quit' and 'restart' commands
 * @ret
 * SPCommand command - a command object representing the user input
 */
SPCommand getUserCommand(SPFiarGame* game, bool isEnded);

/*
 * initializeLevel()
 * Initializes a game in accordance to user's desired level.
 * @param game - current game
 * @ret level - the desired level
 */
int initalizeLevel(SPFiarGame* game);

/*
 * playGame()
 * Handles gameplay
 * @param game- current game
 * @ret num - returns 1 when game is over
 */
int playGame(SPFiarGame* game);



#endif

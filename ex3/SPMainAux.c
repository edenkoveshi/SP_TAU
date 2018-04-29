/*
 * SPMainAux.c
 *
 *  Created on: 9 αιεπι 2017
 *      Author: Eden
 */
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include "SPMiniMax.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SZ 1024

void quit(SPFiarGame* game)
{
	spFiarGameDestroy(game);
	printf("Exiting...\n");
}

void endingGame(SPFiarGame* game){
	char win = spFiarCheckWinner(game);
	spFiarGamePrintBoard(game);
		if (win==SP_FIAR_GAME_PLAYER_1_SYMBOL)
			printf("Game over: you win\n");
		else if(win==SP_FIAR_GAME_PLAYER_2_SYMBOL)
			printf("Game over: computer wins\n");
		else
			printf("Game over: it\'s a tie\n");
		printf("Please enter \'quit\' to exit or \'restart\' to start a new game!\n");
		//game->currentPlayer =SP_FIAR_GAME_PLAYER_1_SYMBOL;
}

SPCommand getUserCommand(SPFiarGame* game, bool isEnded){
	char* input= (char*) malloc(sizeof(char)*MAX_INPUT_SZ);
	SPCommand command={SP_MEMORY_FAILURE,true,0};
	if (input == null){
		printf("Error: getUserCommand has failed");
		return command;
	}
	fgets (input, MAX_INPUT_SZ, stdin);
	// Remove trailing newline, if there.
	if ((strlen(input)>0) && (input[strlen (input) - 1] == '\n'))
				input[strlen (input) - 1] = '\0';

	command = spParserPraseLine(input);
	//validates command
	switch(command.cmd){
		case SP_INVALID_LINE:
			printf("Error: invalid command\n");
			free(input);
			return getUserCommand(game, isEnded);
			break;
		case SP_UNDO_MOVE:
			if (spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_NO_HISTORY){
				printf("Error: cannot undo previous move!\n");
				free(input);
				return getUserCommand(game, isEnded);
			}
			break;
		case SP_ADD_DISC:
			if (isEnded){
				printf("Error: the game is over\n");
				free(input);
				return getUserCommand(game, isEnded);
			}
			else if (!command.validArg){
				printf("Error: column number must be in range 1-7\n");
				free(input);
				return getUserCommand(game, isEnded);
			}
			else if (!spFiarGameIsValidMove(game,command.arg-1)){
				printf("Error: column %d is full\n", command.arg);
				free(input);
				return getUserCommand(game, isEnded);
			}
			break;
		case SP_SUGGEST_MOVE:
			if (isEnded){
				printf("Error: the game is over\n");
				free(input);
				return getUserCommand(game, isEnded);
			}
			break;
		default :
			break;
	}
	free(input);
	return command;
}


int initalizeLevel(SPFiarGame* game){
	char* input= (char*) malloc(sizeof(char)*MAX_INPUT_SZ);
	if (input == null){
			printf("Error: initalizeLevel has failed");
			return 0;
	}
	printf("Please enter the difficulty level between [1-7]:\n");
	fgets (input, MAX_INPUT_SZ, stdin);
	// Remove trailing newline, if there.
	if ((strlen(input)>0) && (input[strlen (input) - 1] == '\n'))
			input[strlen (input) - 1] = '\0';

	if(spParserIsInt(input)){
			int x = atoi(input);
			if (x<0 || x>7)
			{
				free(input);
				printf("Error: invalid level (should be between 1 to 7)\n");
				return initalizeLevel(game);
			}
			else{
				game -> level = x;
				free(input);
				return 1;
			}
		}
	else
		{
			if(strcmp(input,"quit") == 0)
			{
				free(input);
				quit(game);
				return 0;
			}
			else
				free(input);
				printf("Error: invalid level (should be between 1 to 7)\n");
				return initalizeLevel(game);
		}
}

int playGame(SPFiarGame* game){
	int suggest = 0;
	bool isEnded =false;
	spFiarGamePrintBoard(game);
	printf("Please make the next move:\n");
	//main game loop
	while (true){

		if(game->currentPlayer==SP_FIAR_GAME_PLAYER_1_SYMBOL || isEnded)
		{
			SPCommand command = getUserCommand(game,isEnded);
			switch(command.cmd){
				case SP_MEMORY_FAILURE:
					spFiarGameDestroy(game);
					return 1;
				case SP_INVALID_LINE:
					break;
				case SP_UNDO_MOVE:
					spFiarGameUndoPrevMove(game);
					if (spFiarGameGetCurrentPlayer(game) == SP_FIAR_GAME_PLAYER_2_SYMBOL)spFiarGameUndoPrevMove(game);
					isEnded = false;
					spFiarGamePrintBoard(game);
					printf("Please make the next move:\n");
					break;
				case SP_ADD_DISC:
					spFiarGameSetMove(game,command.arg-1);
					if(spFiarCheckWinner(game)){
						isEnded = true;
						endingGame(game);
					}
					break;
				case SP_SUGGEST_MOVE:
					suggest =spMinimaxSuggestMove(game,game->level);
					if (suggest == -1){
						spFiarGameDestroy(game);
						return 1;
					}
					printf("Suggested move: drop a disc to column %d\n", suggest+1);
					break;
				case SP_QUIT:
					quit(game);
					return 1;
				case SP_RESTART:
					printf("Game restarted!\n");
					spFiarGameDestroy(game);
					return 0;
			}
		}
		else{
			suggest = spMinimaxSuggestMove(game,game->level);
			if (suggest == -1){
				spFiarGameDestroy(game);
				return 1;
			}
			spFiarGameSetMove(game,suggest);
			printf("Computer move: add disc to column %d\n",suggest+1);
			if(spFiarCheckWinner(game)){
				isEnded = true;
				endingGame(game);
			}
			else{
				spFiarGamePrintBoard(game);
				printf("Please make the next move:\n");
			}
		}

	}
	return 1;
}

void startGame(){

	SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE);
	if (game==null) return;
	if (initalizeLevel(game)){
		 int endGame = playGame(game);
		 if (!endGame) startGame();
	}
	return;
}

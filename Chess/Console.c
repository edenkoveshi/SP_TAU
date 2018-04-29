/*
 * Console.c
 *
 *  Created on: 11 Aug 2017
 *      Author: amir
 */

#include "Console.h"

void playConsole(){
	Game* game = CreateGame(1,2,1);
	SP_BUFF_SET();
	if (game == NULL){
		printEr("playConsole");
		return;
	}
	game = restartGame(game);
	if (game)
		playGame(game);
	else{
		quitGame(game);
	}
	return;
}

CHCommand getUserCommand(){
	CHCommand userCmd;
	char* input=(char*) malloc(MAX_LEN);
	if (input == NULL){
		printEr("getUserCommand");
		return  failureCmd();
	}
	fgets (input, MAX_LEN, stdin);
	if(input==NULL){
		printEr("getUserCommand");
		return failureCmd();
	}
	int len=strlen(input);
	// Remove trailing newline, if exists.
	if ((len>0) && (input[len - 1] == '\n'))
		input[len - 1] = '\0';
	userCmd = parseLine(input);
	free (input);
	return userCmd;
}
Game* restartGame(Game* game){
	CHCommand userCmd;
	char* msg;
	Game* copy = NULL;
	InitializeBoard(game);
	game-> mode = 1;
	game-> diff = 2;
	game-> usercol = 1;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	do{
		userCmd =  getUserCommand();
		switch (userCmd.cmd){
			case(CH_SETTINGS_GAME_MODE):
				if (userCmd.validArgs){
					game->mode = userCmd.mode;
					msg = game->mode == 1 ? "1 player" : "2 players";
					printf("Game mode is set to %s\n",msg);
				}
				else
					printf("Wrong game mode\n");
				break;
			case(CH_SETTINGS_DIFFICULTY):
				if (game->mode == 2){
					printf("Invalid command\n");
				}
				else if (userCmd.validArgs){
					//if (userCmd.diff == 5)
					//printf("Expert level not supported, please choose a value between 1 to 4:\n");
					switch(userCmd.diff){
					case(1):
							msg = "noob";
							break;
					case(2):
							msg = "easy";
							break;
					case(3):
							msg = "moderate";
							break;
					case(4):
							msg = "hard";
							break;
					default:
							msg = "expert";
							break;
					}
					game->diff = userCmd.diff;
					printf("Difficulty is set to %s\n",msg);
				}
				else
					printf("Wrong difficulty level. the value should be between 1 to 5\n");
				break;
			case(CH_SETTINGS_USER_COLOR):
				if (game->mode == 2){
					printf("Invalid command\n");
				}
				else if (userCmd.validArgs){
					game -> usercol = userCmd.color;
				}
				else
					printf("Wrong color. the value should be 0 or 1\n");
				break;
			case(CH_SETTINGS_DEFAULT):
					game->mode = 1;
					game->usercol = 1;
					game->diff =2;
					break;
			case(CH_SETTINGS_LOAD):
				copy = Load(userCmd.path);
				destroyCommand(&userCmd);
				if (copy == NULL){
					printf("Error: File doesn't exist or cannot be opened\n");
				}
				else{
					 copyGameData(game,copy);
					 DestroyGame(copy);
				}
				break;
			case(CH_SETTINGS_PRINT):
				printSettings(game);
				break;
			case(CH_QUIT):
				printf("Exiting...\n");
				return 0;
			case(CH_START):
				return game;
			case(CH_MEMORY_FAILURE):
				return 0;
			default:
				// need to update
				printf("Invalid command\n");
		}
	}while(true);
	return game;
}


void playGame(Game* game){
	char* color;
	while(STATUS == PLAYING || STATUS == CHECK){
		if (STATUS == CHECK){
			color = (game->turn==0 ? "black" : "white");
			if (MODE == 2 || TURN != USERCOL)
				printf("Check: %s king is threatened!\n",color);
			else
				printf("Check!\n");
		}
		if (MODE == 2 || TURN == USERCOL){
				if (!playUserTurn(game))
					break;
		}
		else if (!playComputerTurn(game))
			break;

	}

	if (STATUS == CHECKMATE){
		color = (TURN == BLACK ? "white": "black");
		printf("Checkmate! %s player wins the game\n",color);
	}
	else if (STATUS == TIE){
		if (MODE == 2 || TURN != USERCOL)
			printf("The game is tied\n");
		else
			printf("The game ends in a tie\n");
	}
	quitGame(game);


}

bool playComputerTurn(Game *game){
	Move move = getComputerMove(game);
	if (move.type == NULL_MOVE)
		return false;
	printComputerMove(move, BOARD[move.source[0]][move.source[1]]);
	MakeMove(game,move);
	updateStatus(game);
	return true;

}

bool playUserTurn(Game* game){
	char* color;
	Move move = getEmptyMove();

	CHCommand userCmd;
	if (game==NULL) return false;
	color = (TURN == BLACK ? "black": "white");
	printBoard(game);
	do{
		printf("%s player - enter your move:\n",color);
		userCmd =  getUserCommand();
		switch (userCmd.cmd){
			case(CH_MOVE):
				if (!userCmd.validArgs)
					printf("Invalid position on the board\n");
				else if	(pieceColor(BOARD[userCmd.move.source[0]][userCmd.move.source[1]])!= TURN)
					printf("The specified position does not contain your piece\n");
				else if(isValidMove(game,&(userCmd.move))&& MakeMove(game,userCmd.move)){
					updateStatus(game);
					return true;
				}
				else
					printf("Illegal move\n");
				break;
			case(CH_GET_MOVES): //BONUS
					if (game->mode == 1 && game->diff<3){
						if (!userCmd.validArgs)
							printf("Invalid position on the board\n");
						else{
							printMoves(game,userCmd.move.source[0],userCmd.move.source[1]);
						}
					}
					else
						printf("get_moves isn't supported in the current game settings.\n");
				break;
			case(CH_SAVE):
				if (!Save(game,userCmd.path))
					printf("File cannot be created or modified\n");
				destroyCommand(&userCmd);
				break;
			case(CH_UNDO):
					if (MODE == 2)
						printf("Undo command not available in 2 players mode\n");
					else{
						move = Undo(game);
						if (move.type == NULL_MOVE)
							printf("Empty history, move cannot be undone\n");
						else{
							printUndoMove(game, move);
							move = Undo(game);
							updateStatus(game);
							printUndoMove(game, move);
							return true;
						}
					}
					break;
			case(CH_RESET):
				printf("Restarting...\n");
				return restartGame(game);
				break;
			case(CH_QUIT):
				printf("Exiting...\n");
				return false;
				break;
			case(CH_MEMORY_FAILURE):
				return false;
			default:
				printf("Invalid command\n");
		}
	}while(true);
	return false;
}

void printUndoMove(Game* game, Move move){
	char* color = pieceColor(BOARD[move.source[0]][move.source[1]]) == BLACK ? "black": "white";
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",color, move.target[0]+1,(char)move.target[1]+65, move.source[0]+1,(char)move.source[1]+65);
}
void quitGame(Game* game){
	if (game != NULL){
		DestroyGame(game);
	}

}

void printSettings(Game *game){
	char* color;
	if (game == NULL)
	return;
	color = (USERCOL == BLACK? "BLACK": "WHITE");
	printf("SETTINGS:\nGAME_MODE: %d\n",MODE);
	if (MODE ==1)
	printf("DIFFICULTY_LVL: %d\nUSER_CLR: %s\n",DIFF,color);
}

void printBoard(Game *game){
	int i,j;
	if (game == NULL)
	return;
	for (i=ROWS; i>0; i--){
		printf("%d| ",i);
		for (j=0; j<COLS; j++){
			printf("%c ",BOARD[i-1][j]);
			}
		printf("|\n");
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
}

void printComputerMove(Move move,Piece piece){
	printf("Computer: move ");
	switch(piece){
		case BPAWN:
		case WPAWN:
			printf("pawn ");
			break;
		case WBISHOP:
		case BBISHOP:
			printf("bishop ");
			break;
		case WROOK:
		case BROOK:
			printf("rook ");
			break;
		case WKNIGHT:
		case BKNIGHT:
			printf("knight ");
			break;
		case WQUEEN:
		case BQUEEN:
			printf("queen ");
			break;
		case WKING:
		case BKING:
			printf("pawn king");
			break;
		default:;
	}
	printf("at <%d,%c> to <%d,%c>\n",move.source[0]+1,(char)move.source[1]+65, move.target[0]+1,(char)move.target[1]+65);
}

void printMoves(Game *game, int row , int col){
	int i=-1;
	char* color = (USERCOL == BLACK? "black": "white");
	Move* moves =  sortedMoves(game,row,col);
	if (moves==NULL){
		printf("The specified position does not contain %s player piece\n",color);
		return;
	}
	if (moves->type ==NULL_MOVE){
			return;
		}
	while (moves[++i].type != NULL_MOVE){
		printf("<%d,%c>",moves[i].target[0]+1,(char)moves[i].target[1]+65);
		if (DIFF < 3){
			switch(moves[i].type){
			case(THR):
					printf("*");
					break;
			case(CAP):
					printf("^*");
					break;
			case(THR_CAP):
					printf("^*");
					break;
			default:;
			}
		}
		printf("\n");
	}
	destroyMove(moves);
}



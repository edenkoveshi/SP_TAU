/*
 * SPFIARParser.c
 *
 *  Created on: 26 May 2017
 *      Author: amir
 */

#include "Parser.h"

void fillGameMode(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
	parser2 = strtok(NULL,"\t\r\n ");
	if (parser2 != NULL){
		if(isInt(parser2))
		{
			cmd->mode = atoi(parser2);
			parser2 = strtok(NULL,"\t\r\n ");
			if((cmd->mode == 1 || cmd->mode==2) && !parser2)
				cmd->validArgs = true;
		}
	}
}

void fillColor(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
	parser2 = strtok(NULL,"\t\r\n ");
	if (parser2 != NULL){
		if(isInt(parser2))
		{
			cmd->color = atoi(parser2);
			parser2 = strtok(NULL,"\t\r\n ");
			if((cmd->color == 0 || cmd->color==1) && !parser2)
				cmd->validArgs = true;
		}
	}
}

void fillDiff(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
	parser2 = strtok(NULL,"\t\r\n ");
	if (parser2 != NULL){
		if(isInt(parser2))
		{
			cmd -> diff = atoi(parser2);
			parser2 = strtok(NULL,"\t\r\n ");
			if((cmd->diff < 6 && cmd->diff > 0) && !parser2)
				cmd->validArgs = true;
		}
	}
}

void fillMove(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
		int placeX,check;
		char placeY;
		parser2 = strtok(NULL,"\t\r\n ");
		if(parser2==NULL) return;
		check = sscanf(parser2,"<%d,%c>", &placeX,&placeY);
		if (check != 2) return;
		else{
			cmd -> move.source[0] = placeX-1;
			cmd -> move.source[1] = (int) placeY - 65;
			if ((cmd -> move.source[0] < 0) || (cmd -> move.source[0] > 7) || (cmd -> move.source[1] < 0) || (cmd -> move.source[1] > 7))
				return;
		}
		parser2 = strtok(NULL,"\t\r\n ");
		if(parser2==NULL) return;
		if (strcmp(parser2,"to") != 0) return;
		parser2 = strtok(NULL,"\t\r\n ");
		if(parser2==NULL) return;
		check = sscanf(parser2,"<%d,%c>", &placeX,&placeY);
		parser2 = strtok(NULL,"\t\r\n ");
		cmd -> move.target[0] = placeX-1;
		cmd -> move.target[1] = (int) placeY - 65;
		if (check == 2 && parser2 == NULL){
				if ((cmd -> move.target[0] > -1) && (cmd -> move.target[0] < 8) && (cmd -> move.target[1] > -1) && (cmd -> move.target[1] < 8))
					cmd->validArgs = true;
		}
}

void fillGetMove(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
	int placeX,check;
	char placeY;
	parser2 = strtok(NULL,"\t\r\n ");
	if(parser2==NULL) return;
	check = sscanf(parser2,"<%d,%c>", &placeX,&placeY);
	parser2 = strtok(NULL,"\t\r\n ");
	if (check == 2 && parser2 == NULL){
		cmd -> move.source[0] = placeX-1;
		cmd -> move.source[1] = (int) placeY - 65;
		if ((cmd -> move.source[0] > -1) && (cmd -> move.source[0] < 8) && (cmd -> move.source[1] > -1) && (cmd -> move.source[1] < 8))
			cmd->validArgs = true;
	}
}

void fillPath(char** parser,CHCommand *cmd ){
	char* parser2 = *parser;
	parser2 = strtok(NULL,"\t\r\n ");
	if(parser2==NULL) return;
	cmd -> path= (char*) malloc(sizeof(char)*(1+strlen(parser2)));
	if (cmd -> path==NULL){ //memory allocation failed
		printEr("parseLine");
		cmd->cmd = CH_MEMORY_FAILURE;
	}
	strcpy(cmd -> path,parser2);
	parser2 = strtok(NULL,"\t\r\n ");
	if (parser2 == NULL) cmd->validArgs = true;
}

bool isInt(const char* str){
	unsigned int i;
	if (strcmp(str,"0")==0) return true;
	for (i = 0; i<strlen(str); i++){
		if ((int)str[i]<48 || (int)str[i]>57) return false;//checks if str[i] is a digit
	}
	return true;
}

CHCommand failureCmd(){
	CHCommand cmd = {CH_MEMORY_FAILURE, false, 0, 0, 0, getEmptyMove(),NULL};
	return cmd;
}

CHCommand parseLine(const char* str){
	CHCommand cmd = {CH_INVALID_LINE, false, 0, 0, 0, getEmptyMove(),NULL};
	if(str==NULL)
	{
		printEr("parseLine");
	}
	int len=strlen(str);
	char* copy= (char*) malloc(sizeof(char)*(1+len));
	if (copy==NULL){ //memory allocation failed
		printEr("parseLine");
		cmd.cmd = CH_MEMORY_FAILURE;
		return cmd;
	}
	strcpy(copy,str);
	char* parser = strtok(copy,"\t\r\n ");
	if (strcmp(parser,"start") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_START;
	}
	else if (strcmp(parser,"quit") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_QUIT;
	}
	else if (strcmp(parser,"reset") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_RESET;
	}
	else if (strcmp(parser,"default") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_SETTINGS_DEFAULT;
	}
	else if (strcmp(parser,"print_setting") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_SETTINGS_PRINT;
	}
	else if (strcmp(parser,"undo") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = CH_UNDO;
	}
	else if (strcmp(parser,"game_mode") == 0){
		cmd.cmd = CH_SETTINGS_GAME_MODE;
		fillGameMode(&parser, &cmd);
	}
	else if (strcmp(parser,"difficulty") == 0){
		cmd.cmd = CH_SETTINGS_DIFFICULTY;
		fillDiff(&parser, &cmd);
	}
	else if (strcmp(parser,"user_color") == 0){
		cmd.cmd = CH_SETTINGS_USER_COLOR;
		fillColor(&parser, &cmd);
	}
	else if (strcmp(parser,"move") == 0){
		cmd.cmd = CH_MOVE;
		fillMove(&parser, &cmd);
	}
	else if (strcmp(parser,"get_moves") == 0){
		cmd.cmd = CH_GET_MOVES;
		fillGetMove(&parser, &cmd);
	}
	else if (strcmp(parser,"load") == 0){
		cmd.cmd = CH_SETTINGS_LOAD;
		fillPath(&parser, &cmd);
	}
	else if (strcmp(parser,"save") == 0){
		cmd.cmd = CH_SAVE;
		fillPath(&parser, &cmd);
	}
	free(copy);
	return cmd;
}

void destroyCommand(CHCommand *cmd){
	if (cmd != NULL && cmd->path != NULL) free(cmd->path);
}



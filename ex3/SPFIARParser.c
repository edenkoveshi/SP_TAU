/*
 * SPFIARParser.c
 *
 *  Created on: 26 May 2017
 *      Author: amir
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SPFIARParser.h"
#define null NULL // used for our convenience

bool spParserIsInt(const char* str){
	if (strcmp(str,"0")==0) return true;
	else return atoi(str);
}

SPCommand spParserPraseLine(const char* str){
	SPCommand cmd = {SP_INVALID_LINE,false,0};
	char* copy= (char*) malloc(sizeof(char)*(1+strlen(str)));
	if (copy==null){ //memory allocation failed
		printf ("Error: spParserPraseLine has failed");
		cmd.cmd = SP_MEMORY_FAILURE;
		return cmd;
	}
	strcpy(copy,str);
	char* parser = strtok(copy,"\t\r\n ");
	if (strcmp(parser,"undo_move") == 0){
		parser = strtok(NULL,"\t\r\n ");
		if (parser == NULL) cmd.cmd = SP_UNDO_MOVE;
	}
	else if (strcmp(parser,"quit") == 0){
			parser = strtok(NULL,"\t\r\n ");
			if (parser == NULL) cmd.cmd = SP_QUIT;
		}
	else if (strcmp(parser,"restart_game") == 0){
			parser = strtok(NULL,"\t\r\n ");
			if (parser == NULL) cmd.cmd = SP_RESTART;
		}
	else if (strcmp(parser,"suggest_move") == 0){
				parser = strtok(NULL,"\t\r\n ");
				if (parser == NULL) cmd.cmd = SP_SUGGEST_MOVE;
			}
	else if (strcmp(parser,"add_disc") == 0){
				parser = strtok(NULL,"\t\r\n ");
				if (parser != NULL){
					cmd.cmd = SP_ADD_DISC;
					if(spParserIsInt(parser))
					{
						cmd.arg = atoi(parser);
						if(cmd.arg>0 && cmd.arg<8)
							cmd.validArg = true;
					}
				}
	}

	free(copy);
	return cmd;

}


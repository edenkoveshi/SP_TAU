#ifndef RPARSER_H_
#define RPARSER_H_


#include "Move.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "GlobalDef.h"

//a type used to represent a command
typedef enum {
	CH_START,
	CH_SETTINGS_GAME_MODE,
	CH_SETTINGS_DIFFICULTY,
	CH_SETTINGS_USER_COLOR,
	CH_SETTINGS_LOAD,
	CH_SETTINGS_DEFAULT,
	CH_SETTINGS_PRINT,

	CH_MOVE,
	CH_GET_MOVES, //BONUS
	CH_SAVE,
	CH_UNDO,
	CH_RESET,
	CH_QUIT,

	CH_INVALID_LINE,
	CH_MEMORY_FAILURE,
} CH_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	CH_COMMAND cmd;
	bool validArgs; //is set to true if the line contains a valid argument
	int mode;
	int diff;
	int color;
	Move move;
	char* path;
} CHCommand;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool isInt(const char* str);

/**
 * Parses a specified line. If the line is a command which has arguments,
 * the relevant arguments are parsed and saved.
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false.
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to CH_INVALID_LINE
 *   validArgs - is set to true if the commands arguments are valid
 *   arg      - the integer argument in case validArg is set to true
 */
CHCommand parseLine(const char* str);

/**
 * free CHCommand-> path if exist.
 */
void destroyCommand(CHCommand*);

/*
 * returns a command that is set to represent a failure command
 */
CHCommand failureCmd();

#endif

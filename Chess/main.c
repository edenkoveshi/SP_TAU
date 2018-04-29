/*
 *main.c
 *
 *  Created on: 3 Aug 2017
 *      Author: amir
 */
#include "Console.h"
#include "GUI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if (argc == 1){
		playConsole();
	}
	else if (argc == 2 && strcmp(argv[1],"-c") == 0)
		playConsole();
	else if (argc ==2 && strcmp(argv[1],"-g") == 0)
		playGUI();
	else
		printf("invalid arguments\n");
	return 1;
}

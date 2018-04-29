#ifndef CHCOMMON_H_
#define CHCOMMON_H_
#include <SDL.h>
#include <SDL_video.h>
#include "GlobalDef.h"
#include "CHGuiState.h"
#include "CHWidget.h"
#include "Game.h"

/*
 * return a new SDL_Rect with same values as the given one
 */
SDL_Rect* chCopyRect(SDL_Rect* src);

/*
 * give the bright pixels in the surface the defined BUTTONS_COLOR
 * other pixels gets the defined MASK_COLOR
 */
void rebuildButton(SDL_Surface* surface);

/*
 * Dark pixels in the surface gets the defined PIECES_COLOR
 * Pixels that close in colour to the mask colour gets the defined MASK_COLOR
 */
void rebuildPiece(SDL_Surface* surface);

/*
 * Add rectangle in the perimeter of the surface
 * the rectangle line width is determined relatively to the defined OFFSET
 * the rectangle color is determined by the given r, g, b values
 */
void addRect(SDL_Surface* surface,int r,int g, int b);
/**
 * Saves a game in one of the 5 game slots available, uses LRU method if all slots are full
 * returns 1 on success, 0 on failure
 * exits game on failure
 **/
int GUISave(CHState* state);
/**
 * Opens a message box upon exitting, allowing a user to save the current game
 * returns 1 if game was saved, 0 otherwise
 **/
int SaveBeforeExit(CHState* state);
/**
 * returns number of saved games
 **/
int SavedGames();
#endif

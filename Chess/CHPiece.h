#ifndef CHPIECE_H_
#define CHPIECE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "CHCommon.h"
#include "CHWidget.h"
#include "Game.h"
#include "GlobalDef.h"
#include "Move.h"
#include "MiniMax.h"

/*
 * CHPiece represents the GUIs Chess piece.
 */
typedef struct ch_piece_t CHPiece;
struct ch_piece_t{
	SDL_Surface* pieceSurface;
	SDL_Texture* pieceTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool isDrag; //help to handle dragging event
	Piece pieceType;
	int pieceRow;
	int pieceCol;
};

/**
 * Constructor for CHWidget with CHPiece data
 * location and sizes of the button depends on the given SDL_Rect
 * Appearance depends on bmp image found at the given 'image' path
 *
 * @return
 * a new CHWidget with CHPiece data
 * or NULL on error.
 *
 */
CHWidget* createPiece(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image, Piece, int, int);

/*
 * CHPiece function for CHWidget generic function "destroyWidget"
 * destroy all memory allocations of CHPiece
 * this function is NULL safe
 */
void destroyPiece(CHWidget*);

/**
 * CHPiece function for CHWidget generic function "handleEvent"
 * The CHState and 'isDrage' variable are updated according to the event
 */
void handlePieceEvenet(CHWidget*, SDL_Event*,CHState*);

/**
 * CHPiece function for CHWidget generic function "drawWidget"
 * redraw the Piece according to its "pieceTexture", "pieceSurface" and "location".
 *
 * @return
 * true on success, false otherwise
 */
bool drawPiece(CHWidget*);

/**
 * CHPiece function for CHWidget generic function "updateAppearance"
 * this is a no-op function.
 * @return
 * true
 */
bool updatePieceAppearance(CHWidget*,CHState*);

/**
 * @return
 * true- if the event was left Mouse Button Down and it occurs on the given CHPiece
 * false- otherwise
 */
bool pieceChosenLeft(CHPiece* data, SDL_Event* event);

/**
 * @return
 * true- if the event was right Mouse Button Down click and it occurs on the given CHPiece
 * false- otherwise
 */
bool pieceChosenRight(CHPiece* data, SDL_Event* event);

/**
 *	This function is called at the end of dragging a the piece.
 *	It updates the state->lastUserMove if the move was valid
 *	If the move was invalid, the piece returned to its original position before being dragged
 */
void updateAfterDrag(CHPiece* data,CHState* state);

#endif

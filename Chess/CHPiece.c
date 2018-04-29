#include "CHPiece.h"

CHWidget* createPiece(SDL_Renderer* windowRender, SDL_Rect* location,const char* image, Piece pieceType, int pieceRow, int pieceCol) {
	CHWidget* res = NULL;
	CHPiece* data = NULL;
	SDL_Surface* loadingSurface = NULL;
	SDL_Texture* pieceTexture = NULL;
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	res = (CHWidget*) malloc(sizeof(CHWidget));
	data = (CHPiece*) malloc(sizeof(CHPiece));
	loadingSurface = SDL_LoadBMP(image);
	if (loadingSurface != NULL)
		rebuildPiece(loadingSurface);
	pieceTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| pieceTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(pieceTexture); ////It is safe to pass NULL
		return NULL ;
	}
	data->pieceType = pieceType;
	data->pieceRow = pieceRow;
	data->pieceCol = pieceCol;
	data->pieceTexture = pieceTexture;
	data->pieceSurface = loadingSurface;
	data->location = chCopyRect(location);
	data->windowRenderer = windowRender;
	data->isDrag = false;
	res->destroyWidget = destroyPiece;
	res->drawWidget = drawPiece;
	res->updateAppearance = updatePieceAppearance;
	res->handleEvent = handlePieceEvenet;
	res->data = data;
	return res;
}

void destroyPiece(CHWidget* src) {
	if (src == NULL ) {
		return;
	}
	CHPiece* castData = (CHPiece*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->pieceTexture);
	SDL_FreeSurface(castData->pieceSurface); //It is safe to pass NULL
	free(castData);
	free(src);
}

void handlePieceEvenet(CHWidget* src, SDL_Event* event, CHState* state){
	CHPiece* data = NULL;
	if (src == NULL || event == NULL|| state == NULL) {
			return;
	}
	data = (CHPiece*) src->data;
	// piece on dragging
	if (state->onDrag && data->isDrag){
		state->redraw = true;
		//continue dragging
		if (event->type == SDL_MOUSEMOTION){
			data->location->x = event->button.x - state->dragX;
			data->location->y = event->button.y - state->dragY;
		}
		//finish dragging
		else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT){
			state->onDrag = false;
			data -> isDrag = false;
			state->redraw = true;
			updateAfterDrag(data,state);

		}
	}

	else if (!state->onDrag && state->game->turn == pieceColor(data-> pieceType)
			&& (state->game->status == PLAYING || state->game->status == CHECK)){
		//start drag
		if (pieceChosenLeft(data,event) && (state->game->mode ==2 || state->game->turn ==  state->game->usercol)){
			state->onDrag = true;
			state->dragX = event->button.x - data->location->x;
			state->dragY = event->button.y - data->location->y;
			data -> isDrag = true;

		}
		//get_moves
		else if (pieceChosenRight(data,event) && state->game->mode == 1 && state->game->diff < 3){
					state->onGetMoves = true;
					state -> getMovesArr = PossibleMoves(state -> game, data->pieceRow, data->pieceCol);
					state->redraw = true;
		}

	}


}

bool drawPiece(CHWidget* src) {
	CHPiece* castData = NULL;
	if (src == NULL ) {
		return false;
	}
	castData = (CHPiece*) src->data;
	SDL_DestroyTexture(castData->pieceTexture);
	SDL_SetColorKey(castData->pieceSurface, SDL_TRUE,SDL_MapRGB(castData->pieceSurface->format,MASK_COLOR));

	castData->pieceTexture = SDL_CreateTextureFromSurface(castData->windowRenderer,castData->pieceSurface);
	if (castData->pieceTexture==NULL){
		return false;
	}
	SDL_RenderCopy(castData->windowRenderer, castData->pieceTexture, NULL,castData->location);

	return true;
}

bool updatePieceAppearance(CHWidget* src,CHState* state){
	if(src&&state) //this is a no-op function (crucial since it implements the widget interface)
		return true;
	return true;
}

bool pieceChosenLeft(CHPiece* data, SDL_Event* event){
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
			SDL_Point point;
			point.x = event->button.x;
			point.y = event->button.y;
			return (SDL_PointInRect(&point, data->location));
		}
	return false;
}

bool pieceChosenRight(CHPiece* data, SDL_Event* event){
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT){
			SDL_Point point;
			point.x = event->button.x;
			point.y = event->button.y;
			return (SDL_PointInRect(&point, data->location));
		}
	return false;
}


void updateAfterDrag(CHPiece* data,CHState* state){
	Move move = getEmptyMove();
	move.source[0] = data->pieceRow;
	move.source[1] = data->pieceCol;
	move.target[0] = (int) 8-(((data -> location->y) + 0.5*GRID_SIZE-2*OFFSET)/GRID_SIZE);
	move.target[1] = (int) (((data -> location->x)-1.5*GRID_SIZE-2*OFFSET)/GRID_SIZE);

	if (isValidPlace(move.target[0],move.target[1]) &&
		isValidMove(state->game,&(move))&& MakeMove(state->game,move)){
		updateStatus(state->game);
		state -> lastUserMove = move;
		state->declareStatus = true;
	}
	else{
		data->location->x = 2*OFFSET+(2+data->pieceCol)*GRID_SIZE;
		data->location->y = 2*OFFSET+(7-data->pieceRow)*GRID_SIZE;
	}

}


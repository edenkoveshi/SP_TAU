#include "CHMainWindow.h"

CHWindow* createMainWindow() {
	CHWindow* res = malloc(sizeof(CHWindow));
	CHMainWindow* data = malloc(sizeof(CHMainWindow));
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL) {
		free(res);
		free(data);
		//We first destroy the renderer
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window); //NULL safe
		printEr("createMainWindow");
		return NULL ;
	}
	data->widgets = NULL;
	data->gameWidgets = NULL;
	data->numOfWidgets = 0;
	data->window = window;
	data->windowRenderer = renderer;
	res->data = (void*) data;
	res->destroyWindow = destroyMainWindow;
	res->drawWindow = drawMainWindow;
	res->handleEventWindow = handleEvenetMainWindow;
	return res;
}

void destroyMainWindow(CHWindow* src) {
	if (src == NULL ) {
		return;
	}
	CHMainWindow* data = (CHMainWindow*) src->data;
	clearWidgets(data);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}

void clearWidgets(CHMainWindow* data){
	int i = 0;
	if (data->widgets!= NULL){
		for (; i < data->numOfWidgets; i++) {
			destroyWidget((data->widgets)[i]);
		}
		free(data->widgets);
		data->widgets = NULL;
	}
	data->numOfWidgets = 0;

	if (data->gameWidgets!= NULL){
		for (i = 0; i < ROWS*COLS; i++){
			destroyWidget((data->gameWidgets)[i]);
		}
		free(data->gameWidgets);
		data->gameWidgets = NULL;
	}
}

bool drawMainWindow(CHWindow* src, CHState* state){
	CHMainWindow* data = NULL;
	int i;
	if (src == NULL ){
			return false;
	}
	data = (CHMainWindow*) src->data;

	// need to change screen and replace widgets
	if (state->screenChanged){
		clearWidgets(data);
		if (state->screen != SCREEN_PLAY)
			setBG(state);
		else
			data->gameWidgets = getPlayPieces(data,state);
		data->widgets =  fillMainWindowWidgets(data,state);
		state->screenChanged = false;
	}

	if (data->widgets == NULL){
		printEr("drawMainWindow");
		return false;
	}

	//draw Background
	if (state->screen == SCREEN_PLAY)
		drawBGgame(data);
	else
		drawBGsettings(data, state);

	// update buttons appearance
	for(i=0;i<data->numOfWidgets;i++){
			data->widgets[i]->updateAppearance(data->widgets[i], state);
	}
	// update pieces position
	updateLastMovement(state->lastUserMove, data);
	updateLastMovement(state->lastComputerMove, data);
	state->lastUserMove.type = state->lastComputerMove.type = NULL_MOVE;
	// draw
	for (i=0; i < data->numOfWidgets; i++) {
			if (data->widgets[i]!=NULL)
				(data->widgets[i])->drawWidget(data->widgets[i]);
		}
	if (state->screen == SCREEN_PLAY){
		for (i=0; i < ROWS*COLS; i++) {
					if (data->gameWidgets[i]!=NULL)
						(data->gameWidgets[i])->drawWidget(data->gameWidgets[i]);
				}
	}
	drawGetMoves(data, state);
	SDL_RenderPresent(data->windowRenderer);
	//after reDrawing
	declareGameStatus(state);
	state ->redraw = false;
	if (addComputerMove(state) && state ->redraw)
		return drawMainWindow(src,state);
	return true;

}

bool handleEvenetMainWindow(CHWindow* src, SDL_Event* event,CHState* state){
	int i;
	CHMainWindow* data = NULL;
	if(src == NULL || event==NULL){
		return false;
	}
	if(event->type == SDL_QUIT){
		state->screen = SCREEN_QUIT;
		return true;
	}
	// clear get moves
	if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT  && state->onGetMoves){
		state->onGetMoves = false;
		destroyMove(state->getMovesArr);
		state->getMovesArr = NULL;
		state -> redraw = true;
		return true;

	}
	data = (CHMainWindow*)src->data;
	//handle button events
	if (!state->onDrag && !state->onGetMoves){
		for(i=0;i<data->numOfWidgets;i++){
			if (data->widgets[i]!=NULL)
				data->widgets[i]->handleEvent(data->widgets[i],event, state);
		}
	}
	//handle pieces events
	if (!state->onGetMoves && data->gameWidgets != NULL){
		for (i=0; i < ROWS*COLS; i++) {
			if (data->gameWidgets[i]!=NULL)
				(data->gameWidgets[i])->handleEvent(data->gameWidgets[i],event, state);
		}
	}
	return true;
}

CHWidget** fillMainWindowWidgets(CHMainWindow* data, CHState* state){
	switch (state->screen){
		case SCREEN_MAIN:
			return getMainButtons(data, state);
			break;
		case SCREEN_MODE:
			return getGameModeButtons(data, state);
			break;
		case SCREEN_DIFF:
			return getDiffButtons(data, state);
			break;
		case SCREEN_USERCOL:
			return getUsercolButtons(data, state);
			break;
		case SCREEN_LOAD:
			return getLoadButtons(data,state);
			break;
		case SCREEN_PLAY:
			return getPlayButtons(data);
			break;
		default:;
	}
	return NULL;
}

CHWidget** getMainButtons(CHMainWindow* data, CHState* state){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
		}
	data ->numOfWidgets = 3;
	widgets = malloc(sizeof(CHWidget*) * 3);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect startR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[0], .y = 2*OFFSET, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect loadR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[2], .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect exitR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[7], .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	widgets[0] = createButton(renderer, &startR, "./buttons/newgame.bmp",BT_NEWGAME);
	widgets[1] = createButton(renderer, &loadR, "./buttons/load.bmp",BT_LOAD);
	widgets[2] = createButton(renderer, &exitR, "./buttons/exit.bmp",BT_EXIT);
	for(; i<data ->numOfWidgets; ++i){
			if (widgets[i] == NULL){
				data->widgets = widgets;
				clearWidgets(data);
				return NULL ;
			}
	}
	return widgets;
}

CHWidget** getGameModeButtons(CHMainWindow* data, CHState* state){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
	}
	data ->numOfWidgets = 5;
	widgets = malloc(sizeof(CHWidget*) * data ->numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect gameModeR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[1], .y = 2*OFFSET+GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect oneR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[2], .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect twoR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[3], .y = 2*OFFSET+3*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect nextR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[6], .y = 2*OFFSET+6*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect backR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[7], .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	widgets[0] = createTitle(renderer, &gameModeR, "./buttons/gamemode.bmp");
	widgets[1] = createButton(renderer, &oneR, "./buttons/oneplayer.bmp",BT_ONEPLAYER);
	widgets[2] = createButton(renderer, &twoR, "./buttons/twoplayers.bmp",BT_TWOPLAYERS);
	widgets[3] = createButton(renderer, &nextR, "./buttons/next.bmp",BT_NEXT);
	widgets[4] = createButton(renderer, &backR, "./buttons/back.bmp",BT_BACK);
	for(; i<data ->numOfWidgets; ++i){
		if (widgets[i] == NULL){
			data->widgets = widgets;
			clearWidgets(data);
			return NULL ;
		}
	}
	return widgets;
}

CHWidget** getDiffButtons(CHMainWindow* data, CHState* state){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
	}
	data ->numOfWidgets = 8;
	widgets = malloc(sizeof(CHWidget*) * data ->numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect diffR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[0], .y = 2*OFFSET, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect noobR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[1], .y = 2*OFFSET+GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect easyR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[2], .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect moderateR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[3], .y = 2*OFFSET+3*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect hardR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[4], .y = 2*OFFSET+4*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect expertR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[5], .y = 2*OFFSET+5*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect nextR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[6], .y = 2*OFFSET+6*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect backR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[7], .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	widgets[0] = createTitle(renderer, &diffR, "./buttons/diff.bmp");
	widgets[1] = createButton(renderer, &noobR, "./buttons/noob.bmp",BT_NOOB);
	widgets[2] = createButton(renderer, &easyR, "./buttons/easy.bmp",BT_EASY);
	widgets[3] = createButton(renderer, &moderateR, "./buttons/moderate.bmp",BT_MODERATE);
	widgets[4] = createButton(renderer, &hardR, "./buttons/hard.bmp",BT_HARD);
	widgets[5] = createButton(renderer, &expertR, "./buttons/expert.bmp",BT_EXPERT);
	widgets[6] = createButton(renderer, &nextR, "./buttons/next.bmp",BT_NEXT);
	widgets[7] = createButton(renderer, &backR, "./buttons/back.bmp",BT_BACK);
	for(; i<data ->numOfWidgets; ++i){
		if (widgets[i] == NULL){
			data->widgets = widgets;
			clearWidgets(data);
			return NULL ;
		}
	}
	return widgets;
}

CHWidget** getUsercolButtons(CHMainWindow* data, CHState* state){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	char *pathBlack = NULL,  *pathWhite = NULL;
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
	}
	data ->numOfWidgets = 5;
	widgets = malloc(sizeof(CHWidget*) * data ->numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	srand ( time(NULL) );
	switch(rand() % 6){
		case 0 :
			pathBlack = "./pieces/M.bmp";
			pathWhite = "./pieces/mw.bmp";
			break;
		case 1 :
			pathBlack = "./pieces/R.bmp";
			pathWhite = "./pieces/rw.bmp";
			break;
		case 2 :
			pathBlack = "./pieces/B.bmp";
			pathWhite = "./pieces/bw.bmp";
			break;
		case 3 :
			pathBlack = "./pieces/N.bmp";
			pathWhite = "./pieces/nw.bmp";
			break;
		case 4 :
			pathBlack = "./pieces/Q.bmp";
			pathWhite = "./pieces/qw.bmp";
			break;
		default :
			pathBlack = "./pieces/K.bmp";
			pathWhite = "./pieces/kw.bmp";
			break;

	}

	SDL_Rect usercolR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[1], .y = 2*OFFSET+GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect blackR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[2], .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_H };
	SDL_Rect whiteR = { .x = 2*OFFSET+GRID_SIZE*(state->settingSpaces[2]+1), .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_H };
	SDL_Rect startR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[6], .y = 2*OFFSET+6*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect backR = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[7], .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	widgets[0] = createTitle(renderer, &usercolR, "./buttons/usercol.bmp");
	widgets[1] = createButton(renderer, &blackR, pathBlack,BT_BLACK);
	widgets[2] = createButton(renderer, &whiteR, pathWhite,BT_WHITE);
	widgets[3] = createButton(renderer, &startR, "./buttons/start.bmp",BT_START);
	widgets[4] = createButton(renderer, &backR, "./buttons/back.bmp",BT_BACK);
	for(; i<data ->numOfWidgets; ++i){
		if (widgets[i] == NULL){
			data->widgets = widgets;
			clearWidgets(data);
			return NULL ;
		}
	}
	return widgets;
}

CHWidget** getPlayButtons(CHMainWindow* data){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
		}
	data ->numOfWidgets = 6;
	widgets = malloc(sizeof(CHWidget*) * data ->numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect restartR = { .x = 2*OFFSET, .y = 2*OFFSET+GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect saveR = { .x = 2*OFFSET, .y = 2*OFFSET+2*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect loadR = { .x = 2*OFFSET, .y = 2*OFFSET+3*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect undoR = { .x = 2*OFFSET, .y = 2*OFFSET+4*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect mainMenuR = { .x = 2*OFFSET, .y = 2*OFFSET+6*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect exitR = { .x = 2*OFFSET, .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };

	widgets[0] = createButton(renderer, &restartR, "./buttons/restart.bmp",BT_RESTART);
	widgets[1] = createButton(renderer, &saveR, "./buttons/save.bmp",BT_SAVE);
	widgets[2] = createButton(renderer, &loadR, "./buttons/load.bmp",BT_LOAD);
	widgets[3] = createButton(renderer, &undoR, "./buttons/undo.bmp",BT_UNDO);
	widgets[4] = createButton(renderer, &mainMenuR, "./buttons/mainmenu.bmp",BT_MAINMENU);
	widgets[5] = createButton(renderer, &exitR, "./buttons/exit.bmp",BT_EXIT);
	for(; i<data ->numOfWidgets; ++i){
			if (widgets[i] == NULL){
				data->widgets = widgets;
				clearWidgets(data);
				return NULL ;
			}
	}
	return widgets;
}

CHWidget** getLoadButtons(CHMainWindow* data, CHState* state)
{
	SDL_Renderer* renderer = data->windowRenderer;
	SDL_Rect slot={.x=0, .y=0, .h=BUTTON_H, .w=BUTTON_W};
	SDL_Rect back =  { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[6], .y = 2*OFFSET+6*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	SDL_Rect start_loaded_game = { .x = 2*OFFSET+GRID_SIZE*state->settingSpaces[7], .y = 2*OFFSET+7*GRID_SIZE, .h = BUTTON_H, .w = BUTTON_W };
	CHWidget** widgets = NULL;
	char *path = (char*) malloc(sizeof(char)*25);
	int i=0;
	if (renderer == NULL || path ==NULL ) {
		return NULL ;
	}
	int saved=SavedGames();
	widgets=malloc((2+saved)*sizeof(CHWidget*));//2 for back and load buttons
	if(widgets==NULL)
	{
		printEr("getLoadButtons");
		return NULL;
	}
	data ->numOfWidgets = saved+2;
	for(i=1; i<saved+1; ++i){
		slot.x = 2*OFFSET+GRID_SIZE*state->settingSpaces[i];
		slot.y = 2*OFFSET+i*GRID_SIZE;
		sprintf(path,"./buttons/slot%d.bmp",i);
		widgets[i-1] = createButton(renderer, &slot,path,BT_GAMESLOT);//won't enter if saved=0
	}
	free(path);
	widgets[saved]=createButton(renderer,&back,"./buttons/back.bmp",BT_BACK);
	widgets[saved+1]=createButton(renderer,&start_loaded_game,"./buttons/start.bmp",BT_START);
	for(i=0; i<data ->numOfWidgets; ++i){
		if (widgets[i] == NULL){
			data->widgets = widgets;
			clearWidgets(data);
			return NULL ;
		}
	}
	return widgets;
}

CHWidget** getPlayPieces(CHMainWindow* data, CHState* state){
	SDL_Renderer* renderer = data->windowRenderer;
	CHWidget** widgets = NULL;
	Piece p= BLANK;
	char* path;
	SDL_Rect piecePlace = { .x = 2*OFFSET+2*GRID_SIZE, .y = 2*OFFSET+7*GRID_SIZE, .h = PIECE_H, .w = PIECE_W };
	int i = 0;
	if (renderer == NULL ) {
			return NULL ;
		}
	widgets = malloc(sizeof(CHWidget*) * ROWS*COLS);
	path = malloc(MAX_LEN);
	if (path == NULL ) {
			return NULL ;
	}
	if (widgets == NULL ) {
		free(path);
		return NULL ;
	}

	for (;i < ROWS*COLS; i++){
		p = (state->game->board)[i/COLS][i%COLS];
		if (p == BLANK)
			widgets[i] = NULL;
		else{
			if (pieceColor(p) == WHITE){
				sprintf(path,"./pieces/%cw.bmp",p);
			}
			else{
				sprintf(path,"./pieces/%c.bmp",p);
			}
			widgets[i] = createPiece(renderer, &piecePlace, path,p,i/COLS,i%COLS);
			if (widgets[i]==NULL){
				i--;
				break;
			}
		}
		piecePlace.x+=GRID_SIZE;
		if (i%COLS == COLS-1){
			piecePlace.x=2*OFFSET+2*GRID_SIZE;
			piecePlace.y-=GRID_SIZE;
		}
	}
	free(path);
	if (i<ROWS*COLS){
		data->widgets = widgets;
		clearWidgets(data);
		return NULL ;
	}
	return widgets;

}

void setBG(CHState* state){
	int i = 0;
	int saved;
	bool* spaces = state->bgSpaces[0];
	bool* sameCol =  state->bgSpaces[1];
	for (; i<ROWS;i++)
		spaces[i] = sameCol[i] = 0;
	switch(state->screen){
			case SCREEN_MAIN:
				spaces[0] = spaces[2] = spaces[7] = 1;
				break;
			case SCREEN_MODE:
				spaces[1] = spaces[2] = spaces[3] = spaces[6] = spaces[7] = 1;
				sameCol[2] = sameCol[3] = 1;
				break;
			case SCREEN_DIFF:
				spaces[0] = spaces[1] = spaces[2] = spaces[3] = 1;
				spaces[4] = spaces[5] = spaces[6] = spaces[7] = 1;
				sameCol[1] = sameCol[2] = sameCol[3] = sameCol[4] = sameCol[5] = 1;
				break;
			case SCREEN_USERCOL:
				spaces[1] = spaces[2] = spaces[6] = spaces[7] = 1;
				sameCol[2] = 1;
				break;
			case SCREEN_LOAD:
				saved=SavedGames();
				i=0;
				while(i<saved-1)
				{
					spaces[i+1]=1;
					sameCol[i+2]=1;
					i++;
				}
				spaces[saved]=1;
				spaces[6]=1;
				spaces[7]=1;
				sameCol[7]=1;
				break;
			case SCREEN_PLAY:
				break;
			default:;
	}
	srand ( time(NULL) );
	for (i=0; i<ROWS;i++)
			state->settingSpaces[i] = sameCol[i] ? state->settingSpaces[i-1] : rand() % 9;
}

void drawBGsettings(CHMainWindow* data, CHState* state){
	SDL_SetRenderDrawColor(data->windowRenderer,BG_COLOR);
	SDL_RenderClear(data->windowRenderer);
	SDL_Rect rec = {.x = WIN_W-(ROWS+1)*GRID_SIZE, .y = 2*OFFSET-GRID_SIZE, .w = GRID_SIZE-2*OFFSET, .h = GRID_SIZE-2*OFFSET};
	int i = 0,j = 0;
	bool* spaces = state->bgSpaces[0];

	//drawBoard
	for (; i<ROWS;i++){
		rec.y+=GRID_SIZE;
		rec.x = 2*OFFSET-GRID_SIZE;
		for (j = 0; j<COLS+2;j++){
				rec.x+=GRID_SIZE;
				if (j==state->settingSpaces[i]&& spaces[i]){
					rec.x+=2*GRID_SIZE;
					j+=2;
				}
				if ((i+j)%2 == 1 )
					SDL_SetRenderDrawColor(data->windowRenderer, SQ_BLACK);
				else SDL_SetRenderDrawColor(data->windowRenderer, SQ_WHITE);
				SDL_RenderFillRect(data->windowRenderer, &rec);
			}
	}
}

void drawBGgame(CHMainWindow* data){
	SDL_SetRenderDrawColor(data->windowRenderer,BG_COLOR);
	SDL_RenderClear(data->windowRenderer);
	SDL_Rect rec = {.x = WIN_W-(ROWS+1)*GRID_SIZE, .y = 2*OFFSET-GRID_SIZE, .w = GRID_SIZE-2*OFFSET, .h = GRID_SIZE-2*OFFSET};
	int i,j;
	//drawBoard

	for (i = 0; i<ROWS;i++){
		rec.y+=GRID_SIZE;
		rec.x = WIN_W-(ROWS+1)*GRID_SIZE;
		for (j = 0; j<COLS;j++){
				rec.x+=GRID_SIZE;
				if ((i+j)%2 == 1 )
					SDL_SetRenderDrawColor(data->windowRenderer, SQ_BLACK);
				else SDL_SetRenderDrawColor(data->windowRenderer, SQ_WHITE);
				SDL_RenderFillRect(data->windowRenderer, &rec);
			}

	}
}

void drawGetMoves(CHMainWindow* data, CHState* state){
	int k = 0;
	SDL_Rect rec = {.x = 0, .y = 0, .w = GRID_SIZE-2*OFFSET, .h = GRID_SIZE-2*OFFSET};
	if (state -> onGetMoves && state->getMovesArr != NULL){
		while ((state->getMovesArr)[k].type != NULL_MOVE){
			rec.y = 2*OFFSET + GRID_SIZE * (ROWS-1-(state->getMovesArr)[k].target[0]);
			rec.x = WIN_W-(COLS-(state->getMovesArr)[k].target[1])*GRID_SIZE;
			switch((state->getMovesArr)[k].type){
				case(REST):
						SDL_SetRenderDrawColor(data->windowRenderer, REST_POSITION);
				break;
				case(CAP):
						SDL_SetRenderDrawColor(data->windowRenderer, CAP_POSITION);
				break;
				default:
						SDL_SetRenderDrawColor(data->windowRenderer, THR_POSITION);
			}
			SDL_SetRenderDrawBlendMode(data->windowRenderer,SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(data->windowRenderer, &rec);
			SDL_SetRenderDrawBlendMode(data->windowRenderer,SDL_BLENDMODE_NONE);
			k++;
		}
	}
}

void updateLastMovement(Move move,CHMainWindow* data ){
	CHPiece* piece = NULL;
	if (move.type != NULL_MOVE){
			piece =(CHPiece*) data->gameWidgets[move.source[0]*COLS+move.source[1]]->data;
			piece->pieceRow = move.target[0];
			piece->pieceCol = move.target[1];
			piece->location->x = 2*OFFSET+(2+piece->pieceCol)*GRID_SIZE;
			piece->location->y = 2*OFFSET+(7-piece->pieceRow)*GRID_SIZE;
			destroyWidget(data->gameWidgets[move.target[0]*COLS+move.target[1]]);
			data->gameWidgets[move.target[0]*COLS+move.target[1]] = data->gameWidgets[move.source[0]*COLS+move.source[1]];
			data->gameWidgets[move.source[0]*COLS+move.source[1]] =NULL;
			move.type =NULL_MOVE;
		}
}

bool addComputerMove(CHState* state){
	Game* game = state->game;
	if (state->screen == SCREEN_PLAY && MODE == 1 && TURN!=USERCOL && (STATUS == PLAYING || STATUS == CHECK)){
		Move move = getComputerMove(state->game);
		if (move.type != NULL_MOVE){
			MakeMove(state->game,move);
			updateStatus(state->game);
			state ->redraw = true;
			state -> lastComputerMove = move;
			state->declareStatus = true;
			SDL_Delay(500);
			return true;
		}
		else{
			printEr("addComputerMove");
			return false;
		}
	}
	return true;
}

void declareGameStatus(CHState* state){
	char message[26], *color;
	Game* game = state->game;
	if(!state->declareStatus || state->screen != SCREEN_PLAY || state->onDrag || state->onGetMoves)
		return;
	switch(STATUS){
	case(CHECK):
		color = (TURN == BLACK ? "black" : "white");
		sprintf(message,"%s king is threatened!",color);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Check!",message,NULL);
		break;
	case(TIE):
		SDL_Delay(1000);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Tie","The game is tied",NULL);
		break;
	case(CHECKMATE):
		SDL_Delay(1000);
		color = (TURN == BLACK ? "white" : "black");
		sprintf(message,"%s player wins the game",color);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Checkmate!",message,NULL);
		break;
	default:
	break;
	}
	state->declareStatus = false;
}

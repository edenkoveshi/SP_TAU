#include "CHButton.h"

CHWidget* createButton(SDL_Renderer* windowRender, SDL_Rect* location,const char* image, BUTTON_TYPE buttonType) {
	CHWidget* res = NULL;
	CHButton* data = NULL;
	SDL_Surface* loadingSurface = NULL;
	SDL_Texture* buttonTexture = NULL;
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	res = (CHWidget*) malloc(sizeof(CHWidget));
	data = (CHButton*) malloc(sizeof(CHButton));
	loadingSurface = SDL_LoadBMP(image);
	if (loadingSurface != NULL){
		if (location->w == BUTTON_W){
			rebuildButton(loadingSurface);
			}
		else{
			rebuildPiece(loadingSurface);
		}
	}
	buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	data->buttonTexture = buttonTexture;
	data->buttonSurface = loadingSurface;
	data->location = chCopyRect(location);
	data->windowRenderer = windowRender;
	data->isActive = true;
	data->buttonType = buttonType;
	res->destroyWidget = destroyButton;
	res->drawWidget = drawButton;
	res->updateAppearance = updateButtonAppearance;
	res->handleEvent = handleButtonEvenet;
	res->data = data;
	return res;
}

void destroyButton(CHWidget* src) {
	if (src == NULL ) {
		return;
	}
	CHButton* castData = (CHButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_FreeSurface(castData->buttonSurface); //It is safe to pass NULL
	free(castData);
	free(src);
}

int slot_chosen=0; //# of chosen game slot, 0 for none
int game_to_load=0; // 1 iff player pressed LOAD button during gameplay

void handleButtonEvenet(CHWidget* src, SDL_Event* event, CHState* state){
	CHButton* data = NULL;
	if (src == NULL || event == NULL|| state == NULL || state->onDrag) {
			return;
	}
	data = (CHButton*) src->data;
	char* gamepath=NULL;
	int response=0;
	if (buttonChosen(data,event)){
		switch(data->buttonType){
			case BT_NEWGAME:
				state->screen = SCREEN_MODE;
				state->screenChanged =true;
				break;
			case BT_START:
				if(data->isActive){
					if(state->screen==SCREEN_LOAD)
					{
						gamepath=malloc(10*sizeof(char));
						if(gamepath==NULL)
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",
														"Loading failed. exiting..",NULL);
							state->screen = SCREEN_QUIT;
						}
						else{
							sprintf(gamepath,"game%d.xml",slot_chosen);//according to buttons set
							DestroyGame(state->game);
							state->game = Load(gamepath);
							slot_chosen = 0;
							free(gamepath);
							state->screen = SCREEN_PLAY;
							state->screenChanged =true;
						}
					}
					else{
						state->screen = SCREEN_PLAY;
						state->screenChanged =true;
					}

				}

				break;
			case BT_NEXT:
				if (state->screen == SCREEN_MODE)state->screen = SCREEN_DIFF;
				else if (state->screen == SCREEN_DIFF)state->screen = SCREEN_USERCOL;
				state->screenChanged =true;
				break;
			case BT_EXIT:
				response=0;
				if(state->screen==SCREEN_PLAY && state->game->status!=CHECKMATE && state->game->
						status!=TIE)
					response=SaveBeforeExit(state);
				if(response!=2) state->screen = SCREEN_QUIT; //2 is cancel
				break;
			case BT_LOAD:
				if(data->isActive){
					game_to_load= (state->screen==SCREEN_PLAY) ? 1: 0;
					state->screen = SCREEN_LOAD;
					state->screenChanged =true;
				}
				break;
			case BT_GAMESLOT:
				slot_chosen=(data->location->y-2*OFFSET)/GRID_SIZE;//this extracts slot number
				break;
			case BT_BACK:
				if (state->screen == SCREEN_MODE)state->screen = SCREEN_MAIN;
				else if (state->screen == SCREEN_MODE)state->screen = SCREEN_MAIN;
				else if (state->screen == SCREEN_DIFF)state->screen = SCREEN_MODE;
				else if (state->screen == SCREEN_USERCOL)state->screen = SCREEN_DIFF;
				else if(state->screen == SCREEN_LOAD)
				{
					if(game_to_load)
						state->screen=SCREEN_PLAY;
					else
						state->screen=SCREEN_MAIN;
					slot_chosen = 0;
				}
				state->screenChanged =true;
				break;
			case BT_ONEPLAYER:
				state->game->mode = 1;
				break;
			case BT_TWOPLAYERS:
				state->game->mode = 2;
				break;
			case BT_NOOB:
				state->game->diff = 1;
				break;
			case BT_EASY:
				state->game->diff = 2;
				break;
			case BT_MODERATE:
				state->game->diff = 3;
				break;
			case BT_HARD:
				state->game->diff = 4;
				break;
			case BT_EXPERT:
				state->game->diff = 5;
				break;
			case BT_BLACK:
				state->game->usercol = 0;
				break;
			case BT_WHITE:
				state->game->usercol = 1;
				break;
			case BT_RESTART:
				InitializeBoard(state->game);
				state->screenChanged =true;
				break;
			case BT_SAVE:
				if(data->isActive)
					GUISave(state);
				break;
			case BT_UNDO:
				if (data->isActive){
					Undo(state->game);
					Undo(state->game);
					state->screenChanged =true;
				}
				break;
			case BT_MAINMENU:
				response=0;
				if(!(state->game->status==CHECKMATE || state->game->status==TIE))
					response=SaveBeforeExit(state);
				if(response!=2){ //if game ended, branch will be entered
					state->screen = SCREEN_MAIN;
					DestroyGame(state->game);
					state->game = CreateGame(1,2,1);
					state->screenChanged =true;
				}
				break;
		}
		state->redraw = true;
	}
}

bool drawButton(CHWidget* src) {
	CHButton* castData = NULL;
	if (src == NULL ) {
		return false;
	}
	castData = (CHButton*) src->data;
	SDL_DestroyTexture(castData->buttonTexture);
	if ((castData->isActive))
		addRect(castData->buttonSurface,BUTTONS_COLOR);
	else
		addRect(castData->buttonSurface,MASK_COLOR);
	SDL_SetColorKey(castData->buttonSurface, SDL_TRUE,SDL_MapRGB(castData->buttonSurface->format,MASK_COLOR));

	castData->buttonTexture = SDL_CreateTextureFromSurface(castData->windowRenderer,castData->buttonSurface);
	if (castData->buttonTexture==NULL){
		return false;
	}
	SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,castData->location);

	return true;
}

bool updateButtonAppearance(CHWidget* src,CHState* state){
	CHButton* data = NULL;
	if (src == NULL || state == NULL ) {
			return false;
	}
	data = (CHButton*) src->data;
	switch(data->buttonType){
		case BT_NEWGAME:
			break;
		case BT_START:
			if (state->screen == SCREEN_MODE && state->game->mode == 1 && !replaceButton(data,"./buttons/next.bmp",BT_NEXT))
				return false;
			if(state->screen == SCREEN_LOAD)
			{
				data->isActive=slot_chosen;
			}
			break;
		case BT_NEXT:
			if (state->game->mode == 2 && !replaceButton(data,"./buttons/start.bmp",BT_START))
				return false;
			break;
		case BT_LOAD:
			data->isActive=(SavedGames()>0);
			break;
		case BT_GAMESLOT:
			data->isActive=((data->location->y-2*OFFSET)/GRID_SIZE ==slot_chosen);
			break;
		case BT_BACK:
			break;
		case BT_ONEPLAYER:
			data->isActive = (state->game->mode == 1);
			break;
		case BT_TWOPLAYERS:
			data->isActive = (state->game->mode == 2);
			break;
		case BT_NOOB:
			data->isActive = (state->game->diff == 1);
			break;
		case BT_EASY:
			data->isActive = (state->game->diff == 2);
			break;
		case BT_MODERATE:
			data->isActive = (state->game->diff == 3);
			break;
		case BT_HARD:
			data->isActive = (state->game->diff == 4);
			break;
		case BT_EXPERT:
			data->isActive = (state->game->diff == 5);
			break;
		case BT_BLACK:
			data->isActive = (state->game->usercol == 0);
			break;
		case BT_WHITE:
			data->isActive = (state->game->usercol == 1);
			break;
		case BT_RESTART:
			break;
		case BT_SAVE:
			data->isActive=!(state->game->status==CHECKMATE || state->game->status==TIE);
			break;
		case BT_UNDO:
			data->isActive = (state->game->mode == 1) && (CHArrayListSize(state->game->history)>1);
			break;
		case BT_MAINMENU:
			break;
		default:;
	}
	return true;
}

bool replaceButton(CHButton* data,const char* image, BUTTON_TYPE buttonType){
		SDL_Surface* loadingSurface =NULL;
		SDL_Texture* buttonTexture = NULL;
		if (data == NULL || image == NULL ) {
			return NULL ;
		}
		SDL_FreeSurface(data->buttonSurface);
		SDL_DestroyTexture(data->buttonTexture);
		//SDL_RenderClear(data->windowRenderer);
		loadingSurface = SDL_LoadBMP(image);
		if (loadingSurface != NULL)
			rebuildButton(loadingSurface);
		buttonTexture = SDL_CreateTextureFromSurface(data->windowRenderer,loadingSurface);
		if (loadingSurface == NULL || buttonTexture == NULL) {
			SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
			SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
			return NULL ;
		}
		data->buttonTexture = buttonTexture;
		data->buttonSurface = loadingSurface;
		data->isActive = true;
		data->buttonType = buttonType;
		return true;
}

bool buttonChosen(CHButton* data, SDL_Event* event){
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT){
			SDL_Point point;

			point.x = event->button.x;
			point.y = event->button.y;
			return (SDL_PointInRect(&point, data->location));
		}
	return false;
}

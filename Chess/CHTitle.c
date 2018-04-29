#include "CHTitle.h"

CHWidget* createTitle(SDL_Renderer* windowRender, SDL_Rect* location,const char* image) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	CHWidget* res = (CHWidget*) malloc(sizeof(CHWidget));
	CHTitle* data = (CHTitle*) malloc(sizeof(CHTitle));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	if (loadingSurface != NULL){
			rebuildButton(loadingSurface);
			SDL_SetColorKey(loadingSurface, SDL_TRUE,SDL_MapRGB(loadingSurface->format,MASK_COLOR));

		}
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| titleTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(titleTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	data->titleTexture = titleTexture;
	data->location = chCopyRect(location);
	data->windowRenderer = windowRender;
	res->destroyWidget = destroyTitle;
	res->drawWidget = drawTitle;
	res->updateAppearance = updateTitleAppearance;
	res->handleEvent = handleTitleEvenet;
	res->data = data;
	return res;
}




void handleTitleEvenet(CHWidget* src, SDL_Event* event, CHState* state){
	if(src&&state&&event)
		src = (1==1) ? src : src;//this is a no-op function (crucial since it implements the widget interface)
}

void destroyTitle(CHWidget* src) {
	if (src == NULL ) {
		return;
	}
	CHTitle* castData = (CHTitle*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->titleTexture);
	free(castData);
	free(src);
}

bool drawTitle(CHWidget* src) {
	CHTitle* castData = NULL;
	if (src == NULL ) {
		return false;
	}
	castData = (CHTitle*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->titleTexture, NULL,castData->location);
	return true;
}

bool updateTitleAppearance(CHWidget* src,CHState* state){
	if(src&&state) //this is a no-op function (crucial since it implements the widget interface)
		return true;
	return true;
}

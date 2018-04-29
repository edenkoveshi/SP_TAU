#include "CHCommon.h"
#define PIXELS ((Uint8*)surface->pixels)

//Helper function
SDL_Rect* chCopyRect(SDL_Rect* src){
	if(src == NULL){
		return NULL;
	}
	SDL_Rect* res = malloc(sizeof(SDL_Rect));
	if(res==NULL){
		return NULL;
	}
	res->h = src->h;
	res->w = src->w;
	res->x = src->x;
	res->y = src->y;
	return res;
}

/*
 * return a "distance" between a given colour in a rgb array format
 * to a pixel at <x,y> color in the given surface
 */
int getDistance(Uint8 *color,SDL_Surface* surface,int x,int y){
	int distance = 0, tmpDis, i = 0;
	for (; i<2; i++){
		tmpDis = (color[i]-PIXELS[(y * surface->w) + x+i]);
		tmpDis = tmpDis<0 ? -tmpDis: tmpDis;
		distance += tmpDis;
	}
	return distance;

}

void addRect(SDL_Surface* surface,int r,int g, int b){
	int lineWidth = 2*OFFSET,x,y;
	//adds vertical lines
	for(x=0;x<surface->w*3;x+=3){
		for(y=0;y<lineWidth*3;y+=3){
			PIXELS[(y * surface->w)+x] = b;
			PIXELS[(y * surface->w)+x+1] = g;
			PIXELS[(y * surface->w)+x+2]  = r;

			PIXELS[(surface->h*3-3-y) * (surface->w)+x] = b;
			PIXELS[(surface->h*3-3-y) * (surface->w)+x+1] = g;
			PIXELS[(surface->h*3-3-y) * (surface->w)+x+2]  = r;
		}
	}
	//adds horizontal lines
	for(x=0;x<lineWidth*3;x+=3){
			for(y=0;y<surface->h*3;y+=3){
				PIXELS[(y * surface->w)+x] = b;
				PIXELS[(y * surface->w)+x+1] = g;
				PIXELS[(y * surface->w)+x+2]  = r;

				PIXELS[((y+3) * surface->w)-3-x] = b;
				PIXELS[((y+3) * surface->w)-2-x] = g;
				PIXELS[((y+3) * surface->w)-1-x]  = r;
			}
		}

}

void rebuildButton(SDL_Surface* surface){
		Uint8 minColor[3] = {255,255,255};
		Uint8 maxColor[3] = {255,255,255};
		Uint8 replacmentColor[3] = {BUTTONS_COLOR};
		Uint8 maskColor[3] = {MASK_COLOR};
		int maxDist, minDist;
		//saving the darkest colour in minColor
		SDL_LockSurface(surface);
		for(int y=0;y<surface->h*3;y+=3) {
			for(int x=0;x<surface->w*3;x+=3) {
				if(getDistance(minColor,surface,x,y)>0){
					minColor[0] = PIXELS[(y * surface->w) + x];
					minColor[1] = PIXELS[(y * surface->w) + x+1];
					minColor[2] = PIXELS[(y * surface->w) + x+2];
				}
			}
		}
		//replacing bright pixels to MASK_COLOR
		// and dark pixels to BUTTONS_COLOR
		for(int y=0;y<surface->h*3;y+=3) {
				for(int x=0;x<surface->w*3;x+=3) {
					maxDist = getDistance(maxColor,surface,x,y);
					minDist = getDistance(minColor,surface,x,y);
					if(maxDist*maxDist > minDist*minDist){
						PIXELS[(y * surface->w) + x] = maskColor[2];
						PIXELS[(y * surface->w) + x+1] = maskColor[1];
						PIXELS[(y * surface->w) + x+2]  = maskColor[0];
					}
					else{
						PIXELS[(y * surface->w) + x] = replacmentColor[2];
						PIXELS[(y * surface->w) + x+1] = replacmentColor[1];
						PIXELS[(y * surface->w) + x+2]  = replacmentColor[0];
					}
				}
		}
		SDL_UnlockSurface(surface);
}

void rebuildPiece(SDL_Surface* surface){
		Uint8 replacmentColor[3] = {PIECES_COLOR};
		Uint8 maskColor[3] = {MASK_COLOR};
		SDL_LockSurface(surface);
		// replacing dark pixels to PIECES_COLOR
		// replacing MASK pixels with the defined MASK_COLOR
		for(int y=0;y<surface->h*3;y+=3) {
			for(int x=0;x<surface->w*3;x+=3) {
				if(getDistance(maskColor,surface,x,y)<50){
					PIXELS[(y * surface->w) + x] = maskColor[2];
					PIXELS[(y * surface->w) + x+1] = maskColor[1];
					PIXELS[(y * surface->w) + x+2] = maskColor[0];
				}
				else if (PIXELS[(y * surface->w) + x] + PIXELS[(y * surface->w) + x+1] +PIXELS[(y * surface->w) + x+2]<50){
					PIXELS[(y * surface->w) + x] = replacmentColor[2];
					PIXELS[(y * surface->w) + x+1] = replacmentColor[1];
					PIXELS[(y * surface->w) + x+2] = replacmentColor[0];
				}
			}
		}

		SDL_UnlockSurface(surface);
}

int GUISave(CHState* state)
{
	int saved=0;
	Game** games= (Game**) malloc((MAX_SLOTS+1)*sizeof(Game*));//+1 for indexing
	char* path=(char*) malloc(10*sizeof(char));
	int i,j;
	for(i=1;i<MAX_SLOTS+1;i++)
	{
		sprintf(path,"game%d.xml",i);
		games[i]=Load(path);
		if(games[i]==NULL) //i now represents number of actual saved files -1
			break;
	}
	for(j=i;j>1;j--)
	{
		if(j!=MAX_SLOTS+1)
		{
			sprintf(path,"game%d.xml",j);
			if(!Save(games[j-1],path))
				{
					for(int k=0;k<j+1;k++)
						DestroyGame(games[k]);
					free(games);
					return 0;
				}
		}
		DestroyGame(games[j-1]);
	}
	saved=Save(state->game,"game1.xml");
	free(path);
	free(games);

	if(saved)
	{
		if(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Success!",
				"Game successfully saved!",NULL)!=0)
			printf("SDL Error");
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Failure!",
									"A problem occured. File can not be saved.",NULL);
	}
	return saved;
}

int SaveBeforeExit(CHState* state)
{
	const SDL_MessageBoxButtonData buttons[] = {
	        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "no" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
	        {0,2,"cancel"}};
	const SDL_MessageBoxData data= {SDL_MESSAGEBOX_INFORMATION,NULL,"Save",
			"Would you like to save before exitting?",SDL_arraysize(buttons),buttons,NULL};
	int buttonid;
	if (SDL_ShowMessageBox(&data, &buttonid) < 0) {
	        SDL_Log("An error occured,game can not be saved");
	}
	if(buttonid==1)
	{
		return GUISave(state);
	}
	if(buttonid==2)
		return 2;
	return 0;
}

int SavedGames()
{
	int i;
	char* filepath=malloc(10*sizeof(char));
	for(i=1;i<MAX_SLOTS+1;i++)
	{
		sprintf(filepath,"game%d.xml",i);
		Game* tmp=Load(filepath);
		if(tmp==NULL)
			break;
		DestroyGame(tmp);
	}
	free(filepath);
	return i-1;
}

/*int* rebuildButton(SDL_Surface* surface){
		Uint8 minColor[3] = {255,255,255};
		Uint8 maxColor[3] = {0,0,0};
		int maxDist, minDist;
		int* retColor = (int*) malloc(sizeof(int)*2);
		//SDL_PixelFormat *fmt=surface->format;

		SDL_LockSurface(surface);
		for(int y=0;y<surface->h*3;y+=3) {
			for(int x=0;x<surface->w*3;x+=3) {
				if(getDistance(minColor,surface,x,y)>0){
					minColor[0] = PIXELS[(y * surface->w) + x];
					minColor[1] = PIXELS[(y * surface->w) + x+1];
					minColor[2] = PIXELS[(y * surface->w) + x+2];
				}
				else if(getDistance(maxColor,surface,x,y)<0){
					maxColor[0] = PIXELS[(y * surface->w) + x];
					maxColor[1] = PIXELS[(y * surface->w) + x+1];
					maxColor[2] = PIXELS[(y * surface->w) + x+2];
					break;
				}
			}
		}

		for(int y=0;y<surface->h*3;y+=3) {
				for(int x=0;x<surface->w*3;x+=3) {
					maxDist = getDistance(maxColor,surface,x,y);
					minDist = getDistance(minColor,surface,x,y);
					if(maxDist*maxDist > minDist*minDist){
						PIXELS[(y * surface->w) + x] = minColor[0];
						PIXELS[(y * surface->w) + x+1] = minColor[1];
						PIXELS[(y * surface->w) + x+2]  = minColor[2];
					}
					else{
						//printf("invert\n");
						PIXELS[(y * surface->w) + x] = maxColor[0];
						PIXELS[(y * surface->w) + x+1] = maxColor[1];
						PIXELS[(y * surface->w) + x+2]  = maxColor[2];
					}
				}
		}
		SDL_UnlockSurface(surface);
		retColor[0] = maxColor[0]+maxColor[1]*256+maxColor[2]*256*256;
		retColor[1] = minColor[0]+minColor[1]*256+minColor[2]*256*256;
		return retColor;

}*/

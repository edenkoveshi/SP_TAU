#include <stdlib.h>
#include "CHWindow.h"

void destroyWindow(CHWindow* src){
	if(!src){
		return;
	}
	src->destroyWindow(src);
}

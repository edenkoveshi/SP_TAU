#include "CHWidget.h"

void destroyWidget(CHWidget* src) {//Making NULL Safe Destroy
	if (src == NULL ) {
		return;
	}
	src->destroyWidget(src);
}

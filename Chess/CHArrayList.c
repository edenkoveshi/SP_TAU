/*
 * CHArrayList.c
 *
 *  Created on: 26 May 2017
 *      Author: amir
 */

#include "CHArrayList.h"

CHArrayList* CHArrayListCreate(int maxSize){
	if (maxSize<=0) return NULL;
	Move* elems = (Move*) malloc(maxSize*sizeof(Move));
	CHArrayList* list = (CHArrayList*) malloc(sizeof(CHArrayList));
	if(list == NULL||elems == NULL){
		printEr("CHArrayListCreate");
		return NULL;
	}
	for (int i = 0; i< maxSize; i++){
		elems[i] = getEmptyMove();
	}
	list->elements=elems;
	list->actualSize=0;
	list->maxSize=maxSize;
	return list;
}

CHArrayList* CHArrayListCopy(CHArrayList* src){

		Move* elements = (Move*) malloc(sizeof(Move)*(src->maxSize));
		CHArrayList* list = (CHArrayList*) malloc(sizeof(CHArrayList));
		if (list==NULL || elements == NULL) {
			printEr("CHArrayListCopy");
			return NULL;
		}
		for (int i = 0; i< src->maxSize; i++){
			elements[i] = src->elements[i];
		}
		list->elements=elements;
		list->actualSize=src->actualSize;
		list->maxSize=src->maxSize;
		return list;
}

void CHArrayListDestroy(CHArrayList* src){
	if  (src != NULL){
		if (src->elements != NULL)
		{
			free(src->elements);
		}
		free(src);
	}
}

CH_ARRAY_LIST_MESSAGE CHArrayListClear(CHArrayList* src){
	if (src == NULL) return CH_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;
	return CH_ARRAY_LIST_SUCCESS;
}

CH_ARRAY_LIST_MESSAGE CHArrayListAddAt(CHArrayList* src, Move elem, int index){

	if (src == NULL || index > src->actualSize) return CH_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == src->maxSize) return CH_ARRAY_LIST_FULL;
	int i= src->actualSize;
	while (i>index){
		src->elements[i]=src->elements[i-1];
		i--;
	}
	src->elements[index] = elem;
	src->actualSize++;
	return CH_ARRAY_LIST_SUCCESS;
}

 CH_ARRAY_LIST_MESSAGE CHArrayListAddFirst(CHArrayList* src, Move elem){
	 return CHArrayListAddAt(src, elem, 0);
 }

CH_ARRAY_LIST_MESSAGE CHArrayListAddLast(CHArrayList* src, Move elem){
	if (src == NULL) return CH_ARRAY_LIST_INVALID_ARGUMENT;
	return CHArrayListAddAt(src, elem, src->actualSize);
}

CH_ARRAY_LIST_MESSAGE CHArrayListRemoveAt(CHArrayList* src, int index){
	if (src == NULL || index >= src->actualSize) return CH_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == 0) return CH_ARRAY_LIST_EMPTY;
	src->elements[index] = getEmptyMove();
	src->actualSize--;
	while (index<src->actualSize){
		src->elements[index]=src->elements[index+1];
		index++;
	}

	return CH_ARRAY_LIST_SUCCESS;
}


CH_ARRAY_LIST_MESSAGE CHArrayListRemoveFirst(CHArrayList* src){
	return CHArrayListRemoveAt(src, 0);
}


CH_ARRAY_LIST_MESSAGE CHArrayListRemoveLast(CHArrayList* src){
	if (src == NULL) return CH_ARRAY_LIST_INVALID_ARGUMENT;
	return CHArrayListRemoveAt(src, (src->actualSize)-1);
}

Move CHArrayListGetAt(CHArrayList* src, int index){
	if (src == NULL || index >= src->actualSize || index < 0) return getEmptyMove();
	return src->elements[index];
}

Move CHArrayListGetFirst(CHArrayList* src){
	return CHArrayListGetAt(src, 0);
}

Move CHArrayListGetLast(CHArrayList* src){
	if (src == NULL) return getEmptyMove();
	return CHArrayListGetAt(src, src->actualSize-1);
}

int CHArrayListMaxCapacity(CHArrayList* src){
	if (src == NULL) return -1;
	return src->maxSize;
}

int CHArrayListSize(CHArrayList* src){
	if (src == NULL) return -1;
	return src->actualSize;
}


bool CHArrayListIsFull(CHArrayList* src){
	if (src == NULL || src->actualSize<src->maxSize) return false;
	return true;
}


bool CHArrayListIsEmpty(CHArrayList* src){
	if (src == NULL || src->actualSize > 0) return false;
	return true;
}




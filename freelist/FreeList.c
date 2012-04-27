/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  Queue.c
*  Implementation file for Queue ADT
***********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "FreeList.h"

/************** Private Structs: not exported *************************************/

typedef struct FreeNode{
   int nodeSize;
   FreeNodeRef nextNode;
   FreeNodeRef prevNode;
} FreeNode;

typedef struct FullNode{
	int nodeSize;
} FullNode;

typedef struct FreeList{
   FreeNodeRef front;
   char *back;
   FreeNodeRef current;
   int numFreeNodes;
   int mode;
} FreeList;


/************** Constructors-Destructors ******************************************/

/*
*  newFreeNode
*  Returns pointer to new FreeNode struct.
*/
FreeNodeRef newFreeNode(int newNodeSize, void *location){
   FreeNodeRef N = location;
   N->nodeSize = newNodeSize;
   N->nextNode = NULL;
   N->prevNode = NULL;
   return(N);
}

/*
*  allocateFullNode
*  Splits FreeNode pointed to by pN to a smaller FreeNode and a FullNode with newNodeSize space.
*  Returns pointer to new FullNode struct (NULL on fail).
*/
FullNodeRef allocateFullNode(FreeListRef L, FreeNodeRef fn, int newNodeSize){
   if( L == NULL){
		printf("FreeList Error: Calling allocateFullNode() with a NULL FreeListRef\n");
		exit(1);
   }
   if( fn == NULL){
		printf("FreeList Error: Calling allocateFullNode() with a NULL FreeNodeRef\n");
		exit(1);
   }
   if( fn->nodeSize < newNodeSize + sizeof(FullNode) ){
		printf("FreeList Error: Calling allocateFullNode() with a newNodeSize greater than the FreeNode nodeSize\n");
		exit(1);
   }
    if( fn->nodeSize == newNodeSize + sizeof(FullNode) ) {
		(fn->prevNode)->nextNode = fn->nextNode;
		(fn->nextNode)->prevNode = fn->prevNode;
		if( getFirst(L) == fn ) {
			L->front = fn->nextNode;
		}
		L->numFreeNodes--;
	} else {
		fn->nodeSize -= newNodeSize + sizeof(FullNode);
	}
	FullNodeRef N = fn + fn->nodeSize;
	memset(N, 0, sizeof(FullNode)+N->nodeSize);
	N->nodeSize = newNodeSize;
	
	return N;
}


/*
*  newFreeList
*  Returns FreeListRef pointing to new FreeList struct.
*  Initializes current and front fields to NULL, sets numFreeNodes field to 1.
*  MODE: 0=first-fit
*		 1=next-fit
*		 2=best-fit
*		 3=worst-fit
*/
FreeListRef newFreeList(int nbytes, int newMode){
   FreeListRef L;
   L = malloc(sizeof(FreeList) + nbytes);
   L->current = L->front = L;
   L->numFreeNodes = 1;
   L->mode = newMode;
   L->back = L + sizeof(FreeList) + nbytes - 1;
   return(L);
}



/***************** Access functions ***********************************************/


/*
*  isFull
*  Returns True if FreeList is full, otherwise returns 0
*/
int isFull(FreeListRef L){
   if( L==NULL ){
      printf("FreeList Error: calling isFull() on NULL FreeListRef\n");
      exit(1);
   }
   return(L->numFreeNodes==0);
}

/*
 * offEnd
 * Returns True if current points to an invalid FreeNodeRef, false otherwise.
 */
int offEnd(FreeListRef L) {
	if( L==NULL ) {
		printf("FreeList Error: calling offEnd() on NULL FreeListRef\n");
		exit(1);
	}
	return (L->current==NULL || L->current < L->front || L->current > L->back);
}

/*
 * atFirst
 * Returns True if current points to the front FreeNodeRef of a valid FreeListRef, false otherwise.
 * Pre: !isFull(L)
 */
int atFirst(FreeListRef L) {
	if( L==NULL ) {
		printf("FreeList Error: calling atFirst() on NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ) {
		printf("FreeList Error: calling atFirst() on a full FreeListRef\n");
		exit(1);
	}
	if( !offEnd(L) ) {
		return( L->current==L->front );
	}
	return 0;
}

/*
*  getFirst
*  Returns the front FreeNodeRef of L.
*  Pre: !isFull(L)
*/
FreeNodeRef getFirst(FreeListRef L) {
   if( L==NULL ){
      printf("FreeList Error: calling getFirst() on NULL FreeListRef\n");
      exit(1);
   }
   if( isFull(L) ){
      printf("FreeList Error: calling getFirst() on an empty FreeListRef\n");
      exit(1);
   }
   return(L->front);
}

/*
 * getBack
 * Returns pointer to the last byte of L.
 */
int getBack(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling getLast() on NULL FreeListRef\n");
		exit(1);
	}
	return(L->back);
}

/*
 * getCurrent
 * Returns the current FreeNodeRef of L.
 * Pre: !isFull(L)
 */
int getCurrent(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling getCurrent() on a NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ){
		printf("FreeList Error: calling getCurrent() on an empty FreeListRef\n");
		exit(1);
	}
	return(L->current);
}

/*
*  getLength
*  Returns the length of L
*/
int getNumFreeNodes(FreeListRef L){
   if( L==NULL ){
      printf("FreeList Error: calling getLength() on NULL FreeListRef\n");
      exit(1);
   }
   return(L->numFreeNodes);
}

/*
*  getMode
*  Returns the mode of L
*  MODE: 0=first-fit
*		 1=next-fit
*		 2=best-fit
*		 3=worst-fit
*/
int getMode(FreeListRef L){
	if( L==NULL ){
		printf("FreeList Error: calling getMode() on NULL FreeListRef\n");
		exit(1);
	}
	return(L->mode);
}




/**************** Manipulation procedures ****************************************/

/*
 * makeFree
 * makes FullNodeRef memory block part of a FreeNodeRef, appropriately merging adjacent FreeNodes
 */
void makeFree(FreeListRef L, FullNodeRef fn) {
	if( L==NULL ){
		printf("FreeList Error: calling makeFree() with a NULL FreeListRef\n");
		exit(1);
	}
	if( fn==NULL ){
		printf("FreeList Error: calling makeFree() with a NULL FullNodeRef\n");
		exit(1);
	}
	if( L->numFreeNodes==0 ){
		L->numFreeNodes = 1;
		FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize, fn);
		L->front = L->current = n;
	} else {
		moveFirst(L);
		FreeNodeRef prior = getCurrent(L);
		while(prior < fn) {
			moveNext(L);
			prior = getCurrent(L);
		}
		FreeNodeRef post = getCurrent(L); /*post FreeNode*/
		movePrev(L);
		prior = getCurrent(L); /*prior FreeNode*/
		
		if( prior + sizeof(FreeNode) + prior->nodeSize == fn){ /*merge prior with adjacent fullnode*/
			prior->nodeSize += sizeof(FullNode) + fn->nodeSize;
			if( fn + sizeof(FullNode) + fn->nodeSize == post ){ /*merge prior with post*/
				prior->nextNode = post->nextNode;
				prior->nodeSize += sizeof(FreeNode) + post->nodeSize;
				L->numFreeNodes -= 1;
			}
		} else if( fn + sizeof(FullNode) + fn->nodeSize == post){ /*merge fullnode with adjacent post*/
			FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize + sizeof(FreeNode) + post->nodeSize, fn);
			n->prevNode = post->prevNode;
			n->nextNode = post->nextNode;
			if( L->numFreeNodes==1 ){
				L->front = L->current = n;
			}
		}
	}
}

/*
 * moveFirst
 * Sets current of L to front
 * Pre: !isFull(L)
 */
void moveFirst(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling moveFirst() on a NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ){
		printf("FreeList Error: calling moveFirst() on an empty FreeListRef\n");
		exit(1);
	}
	L->current = L->front;
}

/*
 * movePrev
 * Sets current of L to the FreeNodeRef previous of current.
 * L->current will point to same FreeNodeRef as before if at beginning of list
 * Pre: !isFull(L) && !offEnd(L)
 */
void movePrev(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling movePrev() on a NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ){
		printf("FreeList Error: calling movePrev() on an empty FreeListRef\n");
		exit(1);
	}
	if( offEnd(L) ){
		printf("FreeList Error: calling movePrev() on an OffEnd() FreeListRef\n");
		exit(1);
	}
	if(L->current == L->current->prevNode) {
		L->current = NULL;
		return;
	}
	L->current = L->current->prevNode;
}

/*
 * moveNext
 * Sets current of L to the FreeNodeRef after current.
 * L->current will point to same FreeNodeRef as before if at end of list.
 * Pre: !isFull(L) && !offEnd(L)
 */
void moveNext(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling moveNext() on a NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ){
		printf("FreeList Error: calling moveNext() on an empty FreeListRef\n");
		exit(1);
	}
	if( offEnd(L) ){
		printf("FreeList Error: calling moveNext() on an OffEnd() FreeListRef\n");
		exit(1);
	}
	if(L->current == L->current->nextNode) {
			L->current = NULL;
			return;
	}
	L->current = L->current->nextNode;
}

/*************** Other Functions *************************************************/

/*
*  printFreeList
*  Prints FreeNode elements in L to stdout.
*/
void printFreeList(FreeListRef L){
   FreeNodeRef N = NULL;
   if( L==NULL ){
      printf("FreeList Error: calling printList() on NULL ListRef\n");
      exit(1);
   }
   printf("(prev, next, nodeSize): ");
   for(N = L->front; N != (N = N->nextNode); ){
      printf("(%d, %d, %d) ", N->prevNode, N->nextNode, N->nodeSize);
   }
   printf("\n");
}
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
*  Returns pointer to new FreeNode struct at location.
*  Don't forget to set nextNode & prevNode!!!
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
		fn->nodeSize = 0;
	} else {
		fn->nodeSize -= newNodeSize + sizeof(FullNode);
	}
	FullNodeRef N = ((int)fn) + fn->nodeSize;
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
   FreeNodeRef N = newFreeNode(nbytes, ((int)L) + sizeof(FreeList));
   N->prevNode = N;
   N->nextNode = N;
   L->current = L->front = N;
   L->numFreeNodes = 1;
   L->mode = newMode;
   L->back = ((int)L) + sizeof(FreeList) + nbytes - 1;
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
 * getFront
 * Returns pointer to the first FreeNode of L
 */
FreeNodeRef getFront(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling getFront() on NULL FreeListRef\n");
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
		printf("FreeList Error: calling getCurrent() on a full FreeListRef\n");
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
 * precondition: fn is a fullnode (not part of a freenode, u asshole)
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
		n->nextNode = n;
		n->prevNode = n;
		L->front = L->current = n;
	} else {
		moveFirst(L);
		FreeNodeRef cur = getCurrent(L);
		while(cur < fn && cur != cur->nextNode) {
			moveNext(L);
			cur = getCurrent(L);
		}
		if( cur + sizeof(FreeNode) + cur->nodeSize == fn){ /*if current is immediately to the left of fullnode, merge*/
			cur->nodeSize += sizeof(FullNode) + fn->nodeSize;
		} else if( fn + sizeof(FullNode) + fn->nodeSize == cur){ /*if current is immediately to the right of fullnode, merge*/
			FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize + sizeof(FreeNode) + cur->nodeSize, fn);
			n->prevNode = cur->prevNode;
			n->nextNode = cur->nextNode;
			if( L->numFreeNodes==1 ){
				L->front = L->current = n;
			}
		} else { /*cur and fn are not adjacent*/
			FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize, fn);
			if(cur < fn) { /*Made new node on the right*/
				if(cur == cur->nextNode) { /*New node is last FreeNode*/
					cur->nextNode = n;
					n->prevNode = cur;
					n->nextNode = n;
				} else { /*New node is in the middle*/
					n->nextNode = cur->nextNode;
					n->prevNode = cur;
					(cur->nextNode)->prevNode = n;
					cur->nextNode = n;
				}
			} else { /*Made new node on the left*/
				if(cur == cur->prevNode) { /*New node is first FreeNode*/
					n->prevNode = n;
					n->nextNode = cur;
					cur->prevNode = n;
					L->front = n;
				} else { /*New node is in the middle*/
					n->prevNode = cur->prevNode;
					n->nextNode = cur;
					(cur->prevNode)->nextNode = n;
					cur->prevNode = n;
				}
			}
			L->numFreeNodes += 1;
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
   int nFN = L->numFreeNodes;
   int i;
   for(i=0, N=L->front; i<nFN; i++, N=N->nextNode) {
		printf("(%d, %d, %d) ", N->prevNode, N->nextNode, N->nodeSize);
   }
   
   printf("\n");
}
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
   int flag;
   FreeNodeRef front;
   char *back;
   FreeNodeRef current;
   int numFreeNodes;
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
*  don't forget to add 4 to pointer after receiving it
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
		if( getFront(L) == fn ) {
			L->front = fn->nextNode;
		}
		L->numFreeNodes--;
		fn->nodeSize = 0;
	} else {
		fn->nodeSize -= newNodeSize + sizeof(FullNode);
		if(fn->nodeSize < sizeof(FreeNode)) {
			printf("FreeList Error: Calling allocateFullNode() results in an insufficiently sized FreeNode!\n");
			exit(1);
		}
	}
	FullNodeRef N = ((int)fn) + fn->nodeSize;
	/*memset(N, 0, sizeof(FullNode)+N->nodeSize); DOING THIS IN FREELISTMMU.C INSTEAD*/
	N->nodeSize = newNodeSize;
	return N;
}


/*
*  newFreeList
*  Returns FreeListRef pointing to new FreeList struct.
*  Initializes current and front fields to NULL, sets numFreeNodes field to 1.
*  FLAG: 0x4=first-fit
*		 0x4|0x08=next-fit
*		 0x4|0x10=best-fit
*		 0x4|0x18=worst-fit
*/
FreeListRef newFreeList(int nbytes, int newFlag){
   FreeListRef L;
   L = malloc(sizeof(FreeList) + nbytes);
   FreeNodeRef N = newFreeNode(nbytes, ((int)L) + sizeof(FreeList));
   N->prevNode = N;
   N->nextNode = N;
   L->current = L->front = N;
   L->numFreeNodes = 1;
   L->flag = newFlag;
   L->back = ((int)L) + sizeof(FreeList) + nbytes - 1;
   moveFirst(L);
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
*  getFlag
*  Returns the flag of L
*  FLAG: 0x4=first-fit
*		 0x4|0x08=next-fit
*		 0x4|0x10=best-fit
*		 0x4|0x18=worst-fit
*/
int getFlag(FreeListRef L){
	if( L==NULL ){
		printf("FreeList Error: calling getFlag() on NULL FreeListRef\n");
		exit(1);
	}
	return(L->flag);
}




/**************** Manipulation procedures ****************************************/

/*
 * makeFree
 * makes FullNodeRef memory block part of a FreeNodeRef, appropriately merging adjacent FreeNodes
 * precondition: fn is a fullnode (not part of a freenode, u asshole)
 * don't forget to subtract 4 bytes before passing it in
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
		FreeNodeRef cur = getFront(L);
		while(cur < fn && cur != cur->nextNode) {
			cur = cur->nextNode;
		}
		if(cur > fn)
			cur = cur->prevNode;
		if( (int)cur + cur->nodeSize == fn){ /*if current is immediately to the left of fullnode, merge*/
			cur->nodeSize += sizeof(FullNode) + fn->nodeSize;
			if((int)cur + cur->nodeSize == cur->nextNode) { /*if merging makes new free node adjacent to next free node, merge those two*/
				cur->nodeSize += (cur->nextNode)->nodeSize;
				if(cur->nextNode == (cur->nextNode)->nextNode) /*we on the edge*/
					cur->nextNode = cur;
				else
					cur->nextNode = (cur->nextNode)->nextNode;
				L->numFreeNodes--;
			}
		} else if( (int)fn + sizeof(FullNode) + fn->nodeSize == cur){ /*if current is immediately to the right of fullnode, merge*/
			FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize + cur->nodeSize, fn);
			(cur->prevNode)->nextNode = n;
			(cur->nextNode)->prevNode = n;
			n->prevNode = cur->prevNode;
			n->nextNode = cur->nextNode;
			if( L->numFreeNodes==1 ){
				L->front = L->current = n;
			}
		} else if( (int)fn + sizeof(FullNode) + fn->nodeSize == cur->nextNode){ /*if current's next is immediately to the right of fullnode, merge*/
			FreeNodeRef n = newFreeNode(sizeof(FullNode) + fn->nodeSize + (cur->nextNode)->nodeSize, fn);
			printf("cur=%d\tn=%d\n", cur, n);
			((cur->nextNode)->prevNode)->nextNode = n;
			((cur->nextNode)->nextNode)->prevNode = n;
			n->prevNode = (cur->nextNode)->prevNode;
			n->nextNode = (cur->nextNode)->nextNode;
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
	L->current = (L->current)->prevNode;
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
		printf("FreeList Error: calling moveNext() on a full FreeListRef\n");
		exit(1);
	}
	if( offEnd(L) ){
		printf("FreeList Error: calling moveNext() on an OffEnd() FreeListRef\n");
		exit(1);
	}
	L->current = (L->current)->nextNode;
}

/*
 * atLast
 * Returns true if current points to last free node, else returns NULL
 */
int atLast(FreeListRef L) {
	if( L==NULL ){
		printf("FreeList Error: calling atLast() on a NULL FreeListRef\n");
		exit(1);
	}
	if( isFull(L) ){
		printf("FreeList Error: calling atLast() on a full FreeListRef\n");
		exit(1);
	}
	if( offEnd(L) ){
		printf("FreeList Error: calling atLast() on an OffEnd() FreeListRef\n");
		exit(1);
	}
	return (L->current)->nextNode == L->current;
}

/*
 * getFreeNodeSize
 * returns the nodeSize of node
 */
int getFreeNodeSize(FreeNodeRef node) {
	if( node==NULL ){
		printf("FreeList Error: calling getFreeNodeSize() on a NULL FreeNodeRef\n");
		exit(1);
	}
	return node->nodeSize;
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
   printf("Printing all %d free nodes in L...\n", L->numFreeNodes);
   printf("(node: size, prev, next): ");
   int nFN = L->numFreeNodes;
   int i;
   for(i=0, N=L->front; i<nFN; i++, N=N->nextNode) {
		printf("(%d: %d, %d, %d) ", N, N->nodeSize, N->prevNode, N->nextNode);
   }
   
   printf("\n");
}
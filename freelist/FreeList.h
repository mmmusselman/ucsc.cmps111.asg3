/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  List.h
*  Header file for List ADT
***********************************************************************************/
#if !defined(_LIST_H_INCLUDE_)
#define _LIST_H_INCLUDE_


/***************************** Exported Types *************************************/
typedef struct FreeList* FreeListRef;
typedef struct FreeNode* FreeNodeRef;
typedef struct FullNode* FullNodeRef;

/************** Constructors-Destructors ******************************************/

/*
*  newFreeNode
*  Returns pointer to new FreeNode struct at location.
*  Don't forget to set nextNode & prevNode!!!
*/
FreeNodeRef newFreeNode(int newNodeSize, void *location);

/*
*  allocateFullNode
*  Splits FreeNode pointed to by pN to a smaller FreeNode and a FullNode with newNodeSize space.
*  Returns pointer to new FullNode struct (NULL on fail).
*  don't forget to add 4 to pointer after receiving it
*/
FullNodeRef allocateFullNode(FreeListRef L, FreeNodeRef fn, int newNodeSize);


/*
*  newFreeList
*  Returns FreeListRef pointing to new FreeList struct.
*  Initializes current and front fields to NULL, sets numFreeNodes field to 1.
*  MODE: 0=first-fit
*		 1=next-fit
*		 2=best-fit
*		 3=worst-fit
*/
FreeListRef newFreeList(int nbytes, int newMode);



/***************** Access functions ***********************************************/


/*
*  isFull
*  Returns True if FreeList is full, otherwise returns 0
*/
int isFull(FreeListRef L);

/*
 * offEnd
 * Returns True if current points to an invalid FreeNodeRef, false otherwise.
 */
int offEnd(FreeListRef L);

/*
 * getFront
 * Returns pointer to the first FreeNode of L
 */
FreeNodeRef getFront(FreeListRef L);

/*
 * getBack
 * Returns pointer to the last byte of L.
 */
int getBack(FreeListRef L);

/*
 * getCurrent
 * Returns the current FreeNodeRef of L.
 * Pre: !isFull(L)
 */
int getCurrent(FreeListRef L);

/*
*  getLength
*  Returns the length of L
*/
int getNumFreeNodes(FreeListRef L);



/**************** Manipulation procedures ****************************************/

/*
 * makeFree
 * makes FullNodeRef memory block part of a FreeNodeRef, appropriately merging adjacent FreeNodes
 * precondition: fn is a fullnode (not part of a freenode, u asshole)
 * don't forget to subtract 4 bytes before passing it in
 */
void makeFree(FreeListRef L, FullNodeRef fn);

/*
 * moveFirst
 * Sets current of L to front
 * Pre: !isFull(L)
 */
void moveFirst(FreeListRef L);

/*
 * movePrev
 * Sets current of L to the FreeNodeRef previous of current.
 * L->current will point to same FreeNodeRef as before if at beginning of list
 * Pre: !isFull(L) && !offEnd(L)
 */
void movePrev(FreeListRef L);

/*
 * moveNext
 * Sets current of L to the FreeNodeRef after current.
 * L->current will point to same FreeNodeRef as before if at end of list.
 * Pre: !isFull(L) && !offEnd(L)
 */
void moveNext(FreeListRef L);

/*
 * atLast
 * Returns true if current points to last free node, else returns NULL
 */
int atLast(FreeListRef L);

/*
 * getFreeNodeSize
 * returns the nodeSize of node
 */
int getFreeNodeSize(FreeNodeRef node);

/*************** Other Functions *************************************************/

/*
*  printFreeList
*  Prints FreeNode elements in L to stdout.
*/
void printFreeList(FreeListRef L);



#endif

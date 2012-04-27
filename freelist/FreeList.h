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
*  Returns pointer to new FreeNode struct.
*  Don't forget to set nextNode & prevNode!!!
*/
FreeNodeRef newFreeNode(int newNodeSize, void *location);

/*
*  allocateFullNode
*  Splits FreeNode pointed to by pN to a smaller FreeNode and a FullNode with newNodeSize space.
*  Returns pointer to new FullNode struct (NULL on fail).
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
 * atFirst
 * Returns True if current points to the front FreeNodeRef of a valid FreeListRef, false otherwise.
 * Pre: !isFull(L)
 */
int atFirst(FreeListRef L);

/*
*  getFirst
*  Returns the front FreeNodeRef of L.
*  Pre: !isFull(L)
*/
FreeNodeRef getFirst(FreeListRef L);

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

/*
*  getMode
*  Returns the mode of L
*  MODE: 0=first-fit
*		 1=next-fit
*		 2=best-fit
*		 3=worst-fit
*/
int getMode(FreeListRef L);




/**************** Manipulation procedures ****************************************/

/*
 * makeFree
 * makes FullNodeRef memory block part of a FreeNodeRef, appropriately merging adjacent FreeNodes
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

/*************** Other Functions *************************************************/

/*
*  printFreeList
*  Prints FreeNode elements in L to stdout.
*/
void printFreeList(FreeListRef L);



#endif

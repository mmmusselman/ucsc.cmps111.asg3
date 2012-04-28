/*Matthew Musselman (W1202889) for P3 in CS111 (Miller, Spring 2012)*/

/**********************************************************************************
*  FreeListMMU.c
*  Implementation file for FreeList MMU
***********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "FreeList.h"
#include "FreeListMMU.h"

/*
*  freelistinit
*/
void *freelistinit(long n_bytes, unsigned int flags) {
	return newFreeList(n_bytes, flags);
}

/*
*  freelistalloc
*/
void *freelistalloc(char *mem, int flag, long n_bytes) {
	FreeListRef L = mem;
	FreeNodeRef cur;
	void *returnPtr;
	returnPtr = NULL;
	if(isFull(L)) {
		printf("FreeListMMU Error: Calling *freelistalloc() on a Full FreeList!\n");
		exit(1);
	}
	switch(flag) {
		case 0x4: /*first-fit*/
			printf("first-fit!\n");
			moveFirst(L);
		case 0x4 | 0x8: /*next-fit*/
			do {
				cur = getCurrent(L);
				if(getFreeNodeSize(cur) >= n_bytes+sizeof(int)) {
					returnPtr = sizeof(int) + (int)allocateFullNode(L, cur, n_bytes);
					break;
				}
				moveNext(L);
			} while(! atLast(L));
			if (returnPtr == NULL) {
				printf("FreeListMMU Error: calling *freelistalloc() on an allocator with insufficient free memory!\n");
				exit(1);
			}
			break;
		case 0x4 | 0x10: /*best-fit*/
			moveFirst(L);
			FreeNodeRef best = NULL;
			do {
				cur = getCurrent(L);
				if(getFreeNodeSize(cur) == n_bytes + sizeof(int))/*found perfect node!*/
					return sizeof(int) + (int)allocateFullNode(L, cur, n_bytes);
				else if(getFreeNodeSize(cur) > n_bytes+sizeof(int)) {
					if(best == NULL)
						best = cur;
					else if(getFreeNodeSize(best) > getFreeNodeSize(cur))
						best = cur;
				}
				moveNext(L);
			} while(! atLast(L));
			if(best == NULL) {
				printf("FreeListMMU Error: calling *freelistalloc() on an allocator with insufficient free memory!\n");
				exit(1);
			}
			returnPtr = sizeof(int) + (int)allocateFullNode(L, best, n_bytes);
			break;
		case 0x4 | 0x18: /*worst-fit*/
			printf("worst-fit!\n");
			break;
	}
	return returnPtr;
}

/*
*	freelistfree
*/
void freelistfree(char *mem, int address) {
	makeFree(mem, address-4);
}


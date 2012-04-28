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
			break;
		case 0x4 | 0x10: /*best-fit*/
			printf("best-fit!\n");
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


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
	switch(flag) {
		case 0x4: /*first-fit*/
			printf("first-fit!\n");
			break;
		case 0x4 | 0x08: /*next-fit*/
			printf("next-fit!\n");
			break;
		case 0x4 | 0x10: /*best-fit*/
			printf("best-fit!\n");
			break;
		case 0x4 | 0x18: /*worst-fit*/
			printf("worst-fit!\n");
			break;
	}
	return NULL;
}

/*
*	freelistfree
*/
void freelistfree(char *mem, int address) {
	return;
}


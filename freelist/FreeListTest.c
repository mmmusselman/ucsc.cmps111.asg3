/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeList.h"

int main(int argc, char* argv[]) {

	FreeListRef L = newFreeList(1024, 12);
	printf("L=%d\n", L);
	printf("2*sizeof(FreeNodeRef)+sizeof(char*)+2*sizeof(int)=%d\n", 2*sizeof(FreeNodeRef)+sizeof(char*)+2*sizeof(int));
	printFreeList(L);
	FreeNodeRef F0 = allocateFullNode(L, getFront(L), 512);
	printf("F0=%d\n", F0);
	printFreeList(L);
	FullNodeRef F1 = allocateFullNode(L, getFront(L), 8);
	printf("F1=%d\n", F1);
	printFreeList(L);
	FullNodeRef F2 = allocateFullNode(L, getFront(L), 8);
	printf("F2=%d\n", F2);
	printFreeList(L);
	makeFree(L, F1);
	printFreeList(L);

   return(0);
}

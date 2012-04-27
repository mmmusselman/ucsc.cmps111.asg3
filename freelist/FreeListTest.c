/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeList.h"

int main(int argc, char* argv[]) {

	FreeListRef L = newFreeList(1024, 12);
	printf("L=newFreeList(1024, 12)=%d\n", L);
	printf("getFront(L)=%d\n", getFront(L));
	printf("getBack(L)=%d\n", getBack(L));
	printFreeList(L);
	FreeNodeRef F0 = allocateFullNode(L, getFront(L), 512);
	printf("F0=allocateFullNode(L, getFront(L), 512)=%d\n", F0);
	printFreeList(L);
	FullNodeRef F1 = allocateFullNode(L, getFront(L), 8);
	printf("F1=allocateFullNode(L, getFront(L), 8)=%d\n", F1);
	printFreeList(L);
	FullNodeRef F2 = allocateFullNode(L, getFront(L), 8);
	printf("F2=allocateFullNode(L, getFront(L), 8)=%d\n", F2);
	printFreeList(L);
	printf("Call makeFree(L, F1)\n");
	makeFree(L, F1);
	printFreeList(L);
	printf("Call makeFree(L, F2)\n");
	makeFree(L, F2);
	printFreeList(L);

   return(0);
}

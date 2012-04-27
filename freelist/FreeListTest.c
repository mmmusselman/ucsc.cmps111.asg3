/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeList.h"

int main(int argc, char* argv[]) {

	FreeListRef L = newFreeList(1024, 12);
	printFreeList(L);
	allocateFullNode(L, getFront(L), 512);
	printFreeList(L);

   return(0);
}

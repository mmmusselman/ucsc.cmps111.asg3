/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeListMMU.h"

int main(int argc, char* argv[]) {
	void *allocator = freelistinit(1024, 0x4|0x10);
	printf("allocator=%d\n", allocator);
	printFreeList(allocator);
	void *region1 = freelistalloc(allocator, 0x4|0x10, 512);
	printf("region1=%d\n", region1);
	printFreeList(allocator);
	printf("calling freelistfree(allocator, (int)region1)...\n");
	freelistfree(allocator, (int)region1);
	printFreeList(allocator);
   return(0);
}

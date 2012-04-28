/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeListMMU.h"

int main(int argc, char* argv[]) {
	void *allocator = freelistinit(1024, 0x4|0x18);
	printf("allocator=%d\n", allocator);
	printFreeList(allocator);
	void *region1 = freelistalloc(allocator, 0x4|0x18, 512);
	printf("region1=%d\n", region1);
	printFreeList(allocator);
	void *region2 = freelistalloc(allocator, 0x4|0x18, 8);
	printf("region2=%d\n", region2);
	printFreeList(allocator);
	void *region3 = freelistalloc(allocator, 0x4|0x18, 8);
	printf("region3=%d\n", region3);
	printFreeList(allocator);
	printf("calling freelistfree(allocator, (int)region2)...\n");
	freelistfree(allocator, (int)region2);
	printFreeList(allocator);
	printf("calling freelistfree(allocator, (int)region1)...\n");
	freelistfree(allocator, (int)region1);
	printFreeList(allocator);
	printf("calling freelistfree(allocator, (int)region3)...\n");
	freelistfree(allocator, (int)region3);
	printFreeList(allocator);
   return(0);
}

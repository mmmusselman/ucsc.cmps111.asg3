/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeListMMU.h"

int main(int argc, char* argv[]) {
	int i;
	int regionSize=100;
	int regionUserSize=regionSize-sizeof(int);
	int numRegions=1024;
	void *region[numRegions];
	void *allocator = freelistinit(regionSize*numRegions, 0x4); //100 KB initialized
	printf("new freelist (first-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	printf("allocating %d regions of size %d bytes...\n", numRegions, regionUserSize);
	sleep(2);
	
	
	//first-free
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full first-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	sleep(3);
	
	//next-free
	allocator = freelistinit(regionSize*numRegions, 0x4|0x08); //100 KB initialized
	printf("new freelist (next-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	printf("allocating %d regions of size %d bytes...\n", numRegions, regionUserSize);
	sleep(2);
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4|0x08, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full next-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	
	
	sleep(3);
	
	//best-free
	allocator = freelistinit(regionSize*numRegions, 0x4|0x10); //100 KB initialized
	printf("new freelist (best-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	printf("allocating %d regions of size %d bytes...\n", numRegions, regionUserSize);
	sleep(2);
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4|0x10, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full best-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	sleep(3);
	
	//worst-free
	allocator = freelistinit(regionSize*numRegions, 0x4|0x18); //100 KB initialized
	printf("new freelist (worst-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	printf("allocating %d regions of size %d bytes...\n", numRegions, regionUserSize);
	sleep(2);
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4|0x18, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full worst-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	//for each: allocate all, free some, allocate more, free all, allocate random, free all
	//after each batch of allocation, print out stats
   return(0);
}




/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/
/*
#include<stdio.h>
#include"FreeListMMU.h"

int main(int argc, char* argv[]) {
	int i;
	int regionSize=100;
	int regionUserSize=regionSize-sizeof(int);
	int numRegions=1024;
	void *region[numRegions];
	void *allocator = freelistinit(regionSize*numRegions, 0x4); /*100 KB initialized*/
/*	printf("new freelist (first-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full first-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	sleep(3);
	
	allocator = freelistinit(regionSize*numRegions, 0x4|0x08); /*100 KB initialized*/
/*	printf("new freelist (next-free) allocator=%d\n", allocator);
	printFreeList(allocator);
	for(i=0; i<numRegions; i++) {
		region[i]=freelistalloc(allocator, 0x4|0x08, regionUserSize);
		printFreeList(allocator);
	}
	printf("Full next-free allocator has %d bytes of user storage allocated, out of a total of %d, for %d-percent efficiency\n", regionUserSize, regionSize, 100*(long)regionUserSize/(long)regionSize);
	sleep(5);
	printf("Freeing all regions...\n");
	sleep(2);
	for(i=1023; i>=0; i--) {
		freelistfree(allocator, region[i]);
	}
	printf("done!\n");
	sleep(1);
	printFreeList(allocator);
	
	
	
	//for each: allocate all, free some, allocate more, free all, allocate random, free all
	//after each batch of allocation, print out stats
   return(0);
}*/

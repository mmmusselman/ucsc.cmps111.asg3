/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeListMMU.h"

int main(int argc, char* argv[]) {
	void *allocator = freelistinit(1024, 0x4);
	void *region1 = freelistalloc(allocator, 0x4, 512);
   return(0);
}

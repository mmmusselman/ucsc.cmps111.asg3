#include <stdio.h>

int main() {
	
	int alloc_id[513];
	int numAllocators = sizeof(alloc_id)/sizeof(int);
	int i;
	
	
	printf("*******************************************************************************\n");
	printf("*\n");
	printf("*		FreeList Performance Test\n");
	printf("*\n");
	printf("*******************************************************************************\n\n\n");
	int alloc_size = 1024;
	unsigned int freeListFlags[4] = {0x4, 0x4|0x08, 0x4|0x10, 0x4|0x18};
	for(i=0; i<numAllocators; i++) {
		if(NULL == (alloc_id[i] = meminit(alloc_size, freeListFlags[i%4], NULL, NULL)) ) {
			printf("Error allocating id %d\n", i);
			exit(1);
		}
		
	}
	
	return 0;
}

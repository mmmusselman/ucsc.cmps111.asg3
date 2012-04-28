#include <stdio.h>

int main() {
	int numAllocators = 512;
	int alloc_id[numAllocators];
	
	int i;
	
	
	printf("*******************************************************************************\n");
	printf("*\n");
	printf("*		FreeList Performance Test\n");
	printf("*\n");
	printf("*******************************************************************************\n\n\n");
	int alloc_size = 1024;
	unsigned int freeListFlags[4] = {0x4, 0x4|0x08, 0x4|0x10, 0x4|0x18};
	for(i=1; i<=numAllocators; i++) {
		printf("i=%d\n", i);
		if(-1 == (alloc_id[i] = meminit(alloc_size, freeListFlags[i%4], NULL, NULL)) )
			exit(1);
	}
	
	return 0;
}

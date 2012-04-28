#include <stdio.h>

int main() {
/*
 * performanceTest Settings
 */
	/*general settings*/
	int numAllocators = 512;
	int alloc_id[numAllocators];
	
	/*FreeList settings*/
	int freeListAllocSize = 1024;
	int freeListRegionSize = 12;
	int freeListNumRegions = freeListAllocSize / (freeListRegionSize+sizeof(int));
	char *freeListRegions[numAllocators][freeListNumRegions/4];
	

/*
 * convenience variables
 */
	int i, j, k;
	
/*
 * FreeList Performance Test
 */
	printf("*******************************************************************************\n");
	printf("*\n");
	printf("*		FreeList Performance Test\n");
	printf("*\n");
	printf("*******************************************************************************\n\n\n");
	unsigned int freeListFlags[4] = {0x4, 0x4|0x08, 0x4|0x10, 0x4|0x18};
	printf("initializing %d FreeList allocators\n", numAllocators);
	printf("\t%d first-fit\n\t%d next-fit\n\t%d best-fit\n\t%d worst-fit\n", numAllocators/4, numAllocators/4, numAllocators/4, numAllocators/4);
	for(i=0; i<numAllocators; i++) {
		if(-1 == (alloc_id[i] = meminit(freeListAllocSize, freeListFlags[i%4], NULL, NULL)) ) {
			printf("Failed to initialize alloc_id[%d].\n", i);
		}
	}
	printf("...success!\n");
	printf("allocating %d-byte regions of each allocator...\n", freeListRegionSize);
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			if(NULL == (freeListRegions[i][j] = memalloc(alloc_id[i], freeListRegionSize))) {
				printf("Failed to allocate %d bytes from alloc_id[%d] after %d identical allocations.\n", freeListRegionSize, i, j);
				break;
			}
		}
	}
	printf("...success!\n");
	printf("checking that each region is entirely zero'd out...\n");
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			for(k=0; k<freeListRegionSize; k++) {
				if(freeListRegions[i][j][k] != 0) {
					printf("Found non-zero component at (%d, %d, %d)!\n", i, j, k);
				}
			}
		}
	}
	
	return 0;
}

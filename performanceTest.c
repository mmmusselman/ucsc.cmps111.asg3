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
	int i, j, k, begin, end, failed;
	
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
	begin=time();
	failed=0;
	for(i=0; i<numAllocators; i++) {
		if(-1 == (alloc_id[i] = meminit(freeListAllocSize, freeListFlags[i%4], NULL, NULL)) ) {
			printf("Test Failed: Could not initialize alloc_id[%d].\n", i);
			failed=1;
		}
	}
	end=time();
	if(failed) printf("...failure! ");
	else printf("...success! ");
	printf("took %d seconds.\n", end-begin);
	printf("allocating %d (maximum number) %d-byte regions from each allocator...\n", freeListNumRegions, freeListRegionSize);
	begin=time();
	failed=0;
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			if(NULL == (freeListRegions[i][j] = memalloc(alloc_id[i], freeListRegionSize))) {
				printf("Test Failed: Could not allocate %d bytes from alloc_id[%d] after %d identical allocations.\n", freeListRegionSize, i, j);
				failed=1;
			}
		}
	}
	end=time();
	if(failed) printf("...failure! ");
	else printf("...success! ");
	printf("took %d seconds.\n", end-begin);
	printf("checking that each region is entirely zero'd out...\n");
	begin=time();
	failed=0;
	//memset(freeListRegions[0][0], 0, 12);
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			for(k=0; k<freeListRegionSize; k++) {
				if(freeListRegions[i][j][k] != 0) {
					printf("Test Failed: Found non-zero component at (%d, %d, %d)!\n", i, j, k);
					failed=1;
				}
			}
		}
	}
	end=time();
	if(failed) printf("...failure! ");
	else printf("...success! ");
	printf("took %d seconds.\n", end-begin);
	/*write 1's*/
	printf("freeing all %d %d-byte regions from each allocator in order allocated...\n", freeListNumRegions, freeListRegionSize);
	begin=time();
	failed=0;
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			printf("(%d, %d)\n", i, j);
			memfree(freeListRegions[i][j]);
		}
	}
	
	printf("allocating %d (maximum number) %d-byte regions from each allocator...\n", freeListNumRegions, freeListRegionSize);
	begin=time();
	failed=0;
	for(i=0; i<numAllocators; i++) {
		for(j=0; j<freeListNumRegions; j++) {
			if(NULL == (freeListRegions[i][j] = memalloc(alloc_id[i], freeListRegionSize))) {
				printf("Test Failed: Could not allocate %d bytes from alloc_id[%d] after %d identical allocations.\n", freeListRegionSize, i, j);
				failed=1;
			}
		}
	}
	end=time();
	if(failed) printf("...failure! ");
	else printf("...success! ");
	printf("took %d seconds.\n", end-begin);
	
	return 0;
}

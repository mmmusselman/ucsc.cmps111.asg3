#include "FreeListMMU.h"
#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>

char *alloc_array[512];
int curr_id = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2)
{
	if(curr_id >= 512) {
		fprintf(stderr, "512 allocators are currently in use\n");
		return -1;
	}
    // Prepare allocators and metadata
    switch (flags)
    {
        // Buddy allocator
        case 0x1: 
            alloc_array[curr_id] = buddyinit(n_bytes, flags, parm1); 
            if (alloc_array[curr_id] == NULL)
            {
                fprintf(stderr, "buddy allocator could not be initialized\n");
                return -1;
            }
            break;
        // Slab allocator
        case 0x2: 
			break;
        // Free-list allocator
        case 0x4: case 0x4 | 0x08:  case 0x4 | 0x10: case 0x4 | 0x18:
			alloc_array[curr_id] = freelistinit(n_bytes, flags);
			if (alloc_array[curr_id] == NULL) {
				fprintf(stderr, "freelist allocator could not be initialized\n");
				return -1;
			}
			break;
        default: fprintf(stderr, "invalid flag\n"); return -1;
    }
    
    // Before returning, prepare the global allocator array index for the next
    // time a new allocator is requested
    ++curr_id;
    // Id for each allocator is 100 + actual index in alloc_array
    return 99 + curr_id;
}

void *memalloc(int handle, long n_bytes)
{
    if (handle - 100 > 511 || handle - 100 < 0)
    {
        fprintf(stderr, "invalid allocator handle passed\n");
        return NULL;
    }
    // Convert the handle into an index recognizeable by the allocator array
    int real_id = handle - 100;
    if (alloc_array[real_id] == NULL)
    {
        printf("alloc_array[%d] has not been initialized\n", real_id);
        return NULL;
    }
    
    // Grab the flag and call the appropriate function for the allocator
    int flag = ((int *)alloc_array[real_id])[0];
    switch (flag)
    {
        case 0x1:
            return buddyalloc(alloc_array[real_id], n_bytes); break;
        case 0x2:
            break;
        case 0x4: case 0x4 | 0x08: case 0x4 | 0x10: case 0x4 | 0x18:
            return freelistalloc(alloc_array[real_id], flag, n_bytes); break;
        default: fprintf(stderr, "invalid flag\n");
    }
    return NULL;
}

void memfree(void *region)
{
    int alloc_id = -1, free_alloc_id=-1;
    long addr_diff = (1 << 30) - 1;
    //printf("addr_diff = %d\n", addr_diff);
    int argi;
    for (argi = 0; argi < 512; ++argi)
    {
        long diff = (long)(region) - (long)(alloc_array[argi]);
        if (diff < addr_diff && diff > 0)
        {
            alloc_id = argi;
            addr_diff = (long)(region) - (long)(alloc_array[argi]);
        }
    }
	
	// set free_alloc_id
	for (argi = 0; argi < 512; argi++) {
		if( (long)region - (long)alloc_array[argi] > 0 ) {
			free_alloc_id=argi;
			break;
		}
	}
    
    // Grab the flag and call the appropriate function for the allocator
    int flag = ((int *)alloc_array[alloc_id])[0];
    switch(flag)
    {
        case 0x1:
            buddyfree(alloc_array[alloc_id], region); break;
        case 0x2:
            break;
        case 0x4: case 0x4 | 0x08: case 0x4 | 0x10: case 0x4 | 0x18:
			printf("alloc_array[free_alloc_id]=%d\n", alloc_array[free_alloc_id]);
            freelistfree(alloc_array[free_alloc_id], region); break;
        default: fprintf(stderr, "invalid flag\n"); 
    }
    //printf("\n");
}


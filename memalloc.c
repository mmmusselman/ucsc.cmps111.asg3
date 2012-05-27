#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>

char *alloc_array[512];
int curr_id = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2)
{
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
        // Free-list allocator
        case 0x4: case 0x4 | 0x08:  case 0x4 | 0x10: case 0x4 | 0x18:
            ((int *)alloc_array[curr_id])[0] = flags; break;
        default: fprintf(stderr, "invalid flag\n"); return -1;
    }
    
    // Debug printing bullshit
    printf("flags = %d\n", ((int *)alloc_array[curr_id])[0]);
    printf("n_bytes = %d\n", ((int *)alloc_array[curr_id])[4]);
    
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
            break;
        default: fprintf(stderr, "invalid flag\n");
    }
    return NULL;
}

void memfree(void *region)
{
    int alloc_id = -1;
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
    printf("alloc_id = %d\n", alloc_id);
    printf("addr_diff = %lu\n", addr_diff);
    
    // Grab the flag and call the appropriate function for the allocator
    int flag = ((int *)alloc_array[alloc_id])[0];
    switch(flag)
    {
        case 0x1:
            buddyfree(alloc_array[alloc_id], region); break;
        case 0x2:
            break;
        case 0x4: case 0x4 | 0x08: case 0x4 | 0x10: case 0x4 | 0x18:
            break;
        default: fprintf(stderr, "invalid flag\n"); 
    }
    
}


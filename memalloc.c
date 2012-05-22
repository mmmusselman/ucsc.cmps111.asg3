#include <stdio.h>
#include <stdlib.h>

char *alloc_array[512];
int curr_id = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2)
{
    printf("n_bytes = %d\n", (int)n_bytes);
    printf("flags = %d\n", flags);
    alloc_array[curr_id] = malloc((sizeof(int) * 2) + (sizeof(char) * n_bytes));

    // If malloc fails, print an error message and return -1
    if (alloc_array[curr_id] == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return -1;
    }
    
    // 1. Save the flag number in the piece of memory itself or return an error
    // 2. Prepare allocators and metadata
    switch (flags)
    {
        // Buddy allocator
        case 0x1: 
            // If requested size is not a power of two, return an error
            if ((n_bytes & (n_bytes - 1)) != 0)
            {
                fprintf(stderr, "requested size for buddy allocator is not a power of 2\n");
                return -1;
            }
        // Slab allocator
        case 0x2: 
        // Free-list allocator
        case 0x4: case 0x4 | 0x08:  case 0x4 | 0x10: case 0x4 | 0x18:
            ((int *)alloc_array[curr_id])[0] = flags; break;
        default: fprintf(stderr, "invalid flag\n"); return -1;
    }
    
    // Save the total number of unallocated number of bytes in the memory slab
    ((int *)alloc_array[curr_id])[4] = n_bytes;
    
    // Debug printing bullshit
    printf("position of array = %d\n", (int)&alloc_array);
    printf("position of alloc 1 = %d\n", (int)&alloc_array[1]);
    printf("position of second int = %d\n", (int)&*(alloc_array)[0]);
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
    if (handle - 100 > 511 || handle - 100 < 0) return NULL;
    return NULL;
}

void memfree(void *region)
{

}


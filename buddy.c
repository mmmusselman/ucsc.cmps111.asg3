#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize a buddy allocator
void *buddyinit(long n_bytes, unsigned int flags, int pg_size)
{
    char *array = NULL;
    // If region to divide is not a power of two, return an error
    // Region is determined by n_bytes / minimum page size
    int num_pages = n_bytes / (1 << pg_size);
    // If the number of total pages is not a power of 2, return NULL (error)
    if ((num_pages & (num_pages - 1)) != 0)
    {
        fprintf(stderr, "requested region for buddy allocator is not a power of 2\n");
        return NULL;
    }
    // Determine size of bitmap
    int bm_size = (2 * num_pages - 1) / 8;
    // Attempt to malloc the needed memory or return a NULL pointer
    array = malloc((sizeof(int) * 3) + (sizeof(char) * (n_bytes + bm_size)));
    if (array == NULL) return NULL;
    // Initialize metadata
    ((int *)array)[0] = flags;
    printf("num_pages = %d\n", num_pages);
    ((int *)array)[4] = num_pages;
    printf("array[4] = %d\n", ((int *)array)[4]);
    ((int *)array)[8] = pg_size;
    return array;
}

// Allocate a section of memory using the buddy allocator
void *buddyalloc(char *mem, long requested)
{
    printf("allocating for a buddy allocator\n");
    // Extract metadata
    //int flag = ((int *)mem)[0];
    int num_pgs = ((int *)mem)[4];
    int pg_size = ((int *)mem)[8];
    int num_levels = lg2(num_pgs);
    if (num_levels == -1) return NULL;
    
    // Find a suitable bitmap level that will minimize external fragmentation
    int argi;
    int level = -1;
    int size = pg_size;
    printf("size = %d\n", size);
    printf("requested = %d\n", (int)requested);
    for (argi = num_levels; ; --argi)
    {
        if (size >= requested)
        {
            level = argi;
            break;
        }
        size *= 2;
    }
    
    printf("level = %d\n", level);
    
    // If level is negative, then there is no level that can handle the 
    // requested number of bytes
    if (level < 0) return NULL;
    
    // Iterate through the bitmap, looking for free space. If there are no 
    // slots available on that level, check all the higher levels. 
    for (; level > 0; --level)
    {
        for (argi = (1 << level) - 1; argi < (1 << (level + 1)) - 1; ++argi)
        {
            printf("argi = %d\n", argi);
            if (bit_is_free(mem, argi))
            {
                printf("bit is free\n");
                int argx = argi;
                while (bit_is_free(mem, argx) && argx >= 0)
                {
                    printf("argx = %d\n", argx);
                    int ind = 12 + (argx / 8);
                    printf("ind = %d\n", ind);
                    mem[ind] = mem[ind] | (1 << (7 - (argx % 8)));
                    printf("mem[%d] = %d\n", ind, mem[ind]);
                    if (argx % 2 == 1) argx = (argx - 1) / 2;
                    else argx = (argx - 2) / 2;
                }
                int offset = pg_size * (1 << (num_levels - level)) * 
                    ((1 << level) - ((1 << (level + 1)) - 1 - argi));
                printf("ret ind = %d\n", 12 + (2 * num_pgs - 1) / 8 + offset);
                return &mem[12 + (2 * num_pgs - 1) / 8 + offset];
            }
        }
    }
    
    // Debug printing bullshit
/*    printf("buddy flag = %d\n", flag);*/
/*    printf("num_pgs = %d\n", num_pgs);*/
/*    printf("pg_size = %d\n", pg_size);*/
/*    printf("lg2(%d) = %d\n", num_pgs, lg2(num_pgs));    */
    
    return NULL;
}

void buddyfree(char *mem, void *region)
{
    printf("freeing memory in a buddy allocator\n");
}

// Helper function to find ln(num)
// Precondition: num must be a power of 2
int lg2(int num)
{
    if ((num & (num - 1)) != 0)
    {
        fprintf(stderr, "number passed into log2 is not a power of 2\n");
        return -1;
    }
    int argi;
    for (argi = 0; ; ++argi)
    {
        if (((num >> argi) & 1) == 1)
            return argi;
    }
}

// Helper function to check if a particular bit in the bitmap is free or used
int bit_is_free(char *array, int num)
{
    return !((array[12 + (num / 8)] >> (7 - (num % 8))) & 1);
}


#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize a buddy allocator
void *buddyinit(long n_bytes, unsigned int flags, int pg_size)
{
    printf("initializing a buddy allocator\n");
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
    int bm_size = (2 * num_pages) / 8;
    int pmap_size = num_pages / 8;
    if (pmap_size == 0) ++pmap_size;
    // Attempt to malloc the needed memory or return a NULL pointer
    array = malloc((sizeof(int) * 3) + (sizeof(char) * (n_bytes + bm_size + pmap_size)));
    if (array == NULL) return NULL;
    // Initialize metadata
    ((int *)array)[0] = flags;
    ((int *)array)[1] = num_pages;
    ((int *)array)[2] = 1 << pg_size;
/*    printf("\n");*/
    return array;
}

// Allocate a section of memory using the buddy allocator
void *buddyalloc(char *mem, long requested)
{
    printf("allocating for a buddy allocator\n");
    // Extract metadata
    //int flag = ((int *)mem)[0];
    int num_pgs = ((int *)mem)[1];
    int pg_size = ((int *)mem)[2];
    int bm_size = (2 * num_pgs) / 8;
    int num_levels = lg2(num_pgs) + 1;
    if (num_levels == -1) return NULL;
    
    // Find a suitable bitmap level that will minimize external fragmentation
    int argi;
    int level = -1;
    int size = pg_size;
    for (argi = num_levels - 1; ; --argi)
    {
        if ((size) >= requested)
        {
            level = argi;
            break;
        }
        size *= 2;
    }
    printf("size = %d\n", size);
    printf("requested = %lu\n", requested);    
/*    printf("level = %d\n", level);*/
    
    // If level is negative, then there is no level that can handle the 
    // requested number of bytes
    if (level < 0) return NULL;
    
    // Iterate through the bitmap, looking for free space. If there are no 
    // slots available on that level, check all the higher levels. 
    for (argi = (1 << level) - 1; argi < (1 << (level + 1)) - 1; ++argi)
    {
/*        printf("argi = %d\n", argi);*/
        if (bit_is_free(mem, argi))
        {
/*            printf("bit is free\n");*/
            int argx = argi;
            set_lower_levels(mem, num_pgs, argx, 1);
            setbit(mem, argx, 0);
            while (bit_is_free(mem, argx) && argx >= 0)
            {
/*                printf("argx = %d\n", argx);*/
                setbit(mem, argx, 1);
                argx = (argx % 2 == 1 ? (argx - 1) / 2 : (argx - 2) / 2);
/*                if (argx % 2 == 1) argx = (argx - 1) / 2;*/
/*                else argx = (argx - 2) / 2;*/
            }
            int rel_ind = (1 << level) - ((1 << (level + 1)) - 1 - argi);
/*            printf("rel_ind = %d\n", rel_ind);*/
            int offset = pg_size * (1 << (num_levels - 1 - level)) * rel_ind;
            int pmap_size = num_pgs / 8;
            if (pmap_size == 0) ++pmap_size;
            int argy = argi;
            while ((2 * num_pgs - 1) - argy > num_pgs) argy = 2 * argy + 1;
            int pmap_ind = (1 << (num_levels - 1)) - ((1 << (num_levels)) - 1 - argy);
/*            printf("argy = %d\n", argy);*/
/*            printf("pmap_ind = %d\n", pmap_ind);*/
/*            printf("test num2 = %d\n", 12 + bm_size + (pmap_ind / 8));*/
            mem[12 + bm_size + (pmap_ind / 8)] |= (1 << pmap_ind % 8);
            
            argy = argi + 1;
            int pm_end;
            while ((2 * num_pgs - 1) - argy > num_pgs) argy = 2 * argy + 1;
            pm_end = (1 << (num_levels - 1)) - ((1 << (num_levels)) - 1 - argy);
            mem[12 + bm_size + (pm_end / 8)] |= (1 << pm_end % 8);
            
            for (argy = pmap_ind + 1; argy < pm_end; ++argy)
                mem[12 + bm_size + (argy / 8)] &= ~(1 << argy % 8);
            
/*            printf("mem[%d] = %d\n", 12 + bm_size + (pmap_ind / 8), mem[12 + bm_size + (pmap_ind / 8)]);*/
/*            printf("ret ind = %d\n", 12 + bm_size + pmap_size + offset);*/
            printf("\n");
            dump_bm(mem);
            return &mem[12 + bm_size + pmap_size + offset];
        }
    }
    printf("\n");
    return NULL;
}

// Free a section of memory based on the address of the object passed in
void buddyfree(char *mem, void *region)
{
    printf("freeing memory in a buddy allocator\n");
    //int argz; for (argz = 0; argz < 20; ++argz) printf("mem[%d] = %d\n", argz, mem[argz]);
/*    dump_bm(mem);*/
/*    dump_pm(mem);*/
    int num_pgs = ((int *)mem)[1];
    int pg_size = ((int *)mem)[2];
    int bm_size = (2 * num_pgs) / 8;
    int pmap_size = num_pgs / 8;
    int num_levels = lg2(num_pgs) + 1;
    char *start_mem = &mem[12 + bm_size + pmap_size];
    int index = (int)(((char *)region - start_mem) / pg_size);
/*    printf("index 1 = %d\n", index);*/
    int blk_size = index;
    int used_pgs = 1;
    for(; !((mem[12 + bm_size + blk_size / 8] >> ((blk_size + 1) % 8)) & 1) && 
        blk_size < num_pgs - index - 1; ++blk_size) 
    {
/*        printf("%d\n", 12 + bm_size + blk_size / 8);*/
        //used_pgs *= 2;
        ++used_pgs;
    }
/*    printf("blk_size = %d\n", blk_size);*/
/*    printf("used_pgs = %d\n", used_pgs);*/
    int level = num_levels - 1 - lg2(used_pgs);
/*    printf("level = %d\n", level);*/
    int abs_ind = (1 << (level + 1)) - 1 - (1 << level) + (index / used_pgs);
/*    printf("abs_ind = %d\n", abs_ind);*/
    set_lower_levels(mem, num_pgs, abs_ind, 0);
    int side = abs_ind % 2;
    while(!bit_is_free(mem, 0) && abs_ind >= 0)
    {
/*        printf("abs_ind 1 = %d\n", abs_ind);*/
        int parent = (side == 1 ? (abs_ind - 1) / 2 : (abs_ind - 1) / 2);
/*        printf("parent = %d\n", parent);*/
        if (bit_is_free(mem, (side == 1 ? (abs_ind + 1) : (abs_ind - 1))))
        {
            setbit(mem, parent, 0);
            abs_ind = parent;
/*            printf("abs_ind 2 = %d\n", abs_ind);*/
            side = abs_ind % 2;
        }
        else
        {
/*            dump_bm(mem);*/
/*            dump_pm(mem);*/
            return;
        }
    }
    if (bit_is_free(mem, 0)) printf("root is free\n");
/*    dump_bm(mem);*/
/*    dump_pm(mem);*/
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
        if (((num >> argi) & 1) == 1) return argi;
    }
}

// Helper function to check if a particular bit in the bitmap is free or used
int bit_is_free(char *array, int num)
{
    return !((array[12 + (num / 8)] >> (7 - (num % 8))) & 1);
}

// Helper function to set a particular bit in the bitmap to a 0 or 1
void setbit(char *array, int num, int state)
{
    if (state == 1) array[12 + (num / 8)] |= (1 << (7 - (num % 8)));
    else array[12 + (num / 8)] &= ~(1 << (7 - (num % 8)));
}

// Helper function to set the value of all children of the specified bit
void set_lower_levels(char *array, int num_pages, int num, int state)
{
    if (num > 2 * num_pages - 1) return; 
    setbit(array, num, state);
    set_lower_levels(array, num_pages, num * 2 + 1, state);
    set_lower_levels(array, num_pages, num * 2 + 2, state);
}

void dump_bm(char *array)
{
    int num_pgs = ((int *)array)[1];
    int bm_size = (2 * num_pgs) / 8;
    int argi;
    int argx;
    for (argi = 12; argi < 12 + bm_size; ++argi)
    {
        printf("array[%d]:\t", argi);
        for (argx = 0; argx < 8; ++argx)
        {
            printf("%4d", (array[argi] >> (7 - argx)) & 1);
        }
        printf("\n");
    }
    printf("\n");
}

void dump_pm(char *array)
{
    int num_pgs = ((int *)array)[1];
    int bm_size = (2 * num_pgs) / 8;
    int pm_size = num_pgs / 8;
    int argi;
    int argx;
    for (argi = 12 + bm_size; argi < 12 + bm_size + pm_size; ++argi)
    {
        printf("array[%d]:\t", argi);
        for (argx = 0; argx < 8; ++argx)
        {
            printf("%4d", (array[argi] >> argx) & 1);
        }
        printf("\n");
    }
    printf("\n");
}


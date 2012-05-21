#include <stdio.h>
#include <stdlib.h>

char *alloc_array[512];
int curr_id = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2)
{
    printf("n_bytes = %d\n", (int)n_bytes);
    alloc_array[curr_id] = malloc(sizeof(int) * (n_bytes + 1));
    char *array = alloc_array[curr_id];
    *(alloc_array + curr_id)[0] = flags;
    printf("%d\n", array[0]);
    *(array + 1) = 3;
    //alloc_array[curr_id + 1] = 'd';
    //*(alloc_array + curr_id)[1] = 5;
    printf("sizeof char = %d\n", sizeof(char));
    printf("position of array = %d\n", &alloc_array);
    printf("position of alloc 1 = %d\n", &alloc_array[1]);
    printf("position of second int = %d\n", &*(alloc_array)[0]);
    printf("flag = %d\n", *(alloc_array + curr_id)[0]);
    printf("int after flag = %d\n", alloc_array[0][1]);
    ++curr_id;
    return 99 + curr_id;
}

void *memalloc(int handle, long n_bytes)
{
    return NULL;
}

void memfree(void *region)
{

}


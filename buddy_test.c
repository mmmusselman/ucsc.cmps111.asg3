#include <stdio.h>

struct test
{
    int numa; int numb; int numc; int numd; int nume;
};

struct lg_test
{
    int num1; int num2; int num3; int num4; int num5; int num6; int num7; int num8;
    int num9; int num10; int num11; int num12; int num13; int num14; int num15; int num16;
    int num17; int num18;
};

struct xlg_test
{
    int num1; int num2; int num3; int num4; int num5; int num6; int num7; int num8;
    int num9; int num10; int num11; int num12; int num13; int num14; int num15; int num16;
    int num17; int num18; int num19; int num20; int num21; int num22; int num23; int num24;
    int num25; int num26; int num27; int num28; int num29; int num30; int num31; int num32;
    int num33; int num34;
};

int main()
{
    int obj_total = 0;
    int alloc_total = 0;
    int frag_total = 0;
    
    int buddy_id = meminit(1 << 10, 0x1, 6, NULL);
    int pg_size = (1 << 6);
    printf("page size of buddy allocator = %d\n", pg_size);
    printf("number of pages in buddy allocator = %d\n", (1 << 10) / (1 << 6));
    
    struct test *test_objs[20];
    struct lg_test *lg_test_objs[20];
    
    int argi;
    int size;
    for (argi = 0; argi < 20; ++argi)
    {
        printf("allocating test object %d\n", argi);
        test_objs[argi] = memalloc(buddy_id, sizeof(struct test));
        if (test_objs[argi] == NULL) break;
        size = pg_size;
        while (size < sizeof(struct test)) size *= 2;
        obj_total += sizeof(struct test);
        alloc_total += size;
        frag_total += size - sizeof(struct test);
        printf("\n");
    }
    
    printf("after allocating as many test objects as possible:\n");
    printf("total bytes allocated for test objects = %d\n", obj_total);
    printf("total bytes allocated for pages        = %d\n", alloc_total);
    printf("total bytes of internal fragmentation  = %d\n", frag_total);
    
    for (argi = 0; argi < 10; ++argi)
    {
        printf("freeing test object %d\n", argi);
        memfree(test_objs[argi]);
        size = pg_size;
        while (size < sizeof(struct test)) size *= 2;
        obj_total -= sizeof(struct test);
        alloc_total -= size;
        frag_total -= size - sizeof(struct test);
        
    }
    
    
    
    return 0;
}

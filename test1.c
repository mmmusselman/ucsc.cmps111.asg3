#include <stdio.h>

int main()
{
    int a = 1;
    int b = meminit(4, 0x2, 0, &a);
    printf("%d\n", b);
    return 0;
}

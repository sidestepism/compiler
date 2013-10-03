#include <stdio.h>

int f(int x, int y, int z);

int main()
{
    int x = 3;
    int y = 4;
    int z = 5;
    printf("f(%d, %d, %d) = %d\n", x, y, z, f(x, y, z));
}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char diagonals[] = { 0x06, 0x07 };
    while (true)
        putchar(diagonals[rand() & 1]);
}

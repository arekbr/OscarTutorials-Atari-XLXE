#include <stdio.h>
#include <stdlib.h>
#include "../common/hw.h"

// With BASIC off, Atari OS and E: still occupy upper memory.
// Leave a small C stack and use the largest contiguous low-RAM area.
#pragma region(main, 0x2080, 0xbc00, , , {code, data, bss, heap, stack})
#pragma stacksize(512)

#define SIZE 33792U

int main(void)
{
    volatile byte *buffer = (volatile byte *)malloc(SIZE);
    if (!buffer)
    {
        puts("Could not allocate 33 KiB.\n");
        return 1;
    }

    for (word i = 0; i < SIZE; i++)
        buffer[i] = (byte)i ^ (i >> 8);
    for (word i = 0; i < SIZE; i++)
        if (buffer[i] != ((byte)i ^ (i >> 8)))
        {
            printf("RAM mismatch at %04x\n", (word)buffer + i);
            return 1;
        }

    printf("33 KiB verified: %04x-%04x\n", (word)buffer, (word)buffer + SIZE - 1);
    free((void *)buffer);
    return 0;
}

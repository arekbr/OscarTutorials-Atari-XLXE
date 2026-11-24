#include <stdio.h>
#include <stdlib.h>
#include "../common/hw.h"

#pragma region(main, 0x2080, 0xbc00, , , {code, data, bss, heap, stack})
#pragma stacksize(512)

// This code and its stack are in low RAM. Interrupts stay masked while
// the OS vectors are hidden. $D000-$D7FF remains the hardware window.
static bool test_hidden_ram(void)
{
    byte port = REG8(0xd301);
    __asm { php
            sei }
    NMIEN = 0;
    REG8(0xd301) = port & 0xfe;

    bool verified = true;
    for (word page = 0xc0; page <= 0xff; page++)
    {
        if (page == 0xd0)
            page = 0xd8;
        volatile byte *data = (volatile byte *)(page << 8);
        for (word i = 0; i < 256; i++)
            data[i] = (byte)i ^ (byte)page;
    }
    for (word page = 0xc0; page <= 0xff; page++)
    {
        if (page == 0xd0)
            page = 0xd8;
        volatile byte *data = (volatile byte *)(page << 8);
        for (word i = 0; i < 256; i++)
            if (data[i] != ((byte)i ^ (byte)page))
                verified = false;
    }

    REG8(0xd301) = port;
    NMIEN = 0x40;
    __asm { plp }
    return verified;
}

int main(void)
{
    volatile byte *low = (volatile byte *)malloc(32768U);
    if (!low)
    {
        puts("Could not allocate 32 KiB.\n");
        return 1;
    }
    for (word i = 0; i < 32768U; i++)
        low[i] = (byte)i ^ (i >> 8);
    bool upper = test_hidden_ram();
    for (word i = 0; i < 32768U; i++)
        if (low[i] != ((byte)i ^ (i >> 8)))
        {
            puts("Low RAM mismatch.\n");
            return 1;
        }
    free((void *)low);
    puts(upper ? "46 KiB verified, OS restored.\n" : "Upper RAM mismatch.\n");
    return upper ? 0 : 1;
}

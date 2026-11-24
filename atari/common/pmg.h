#ifndef TUTORIAL_PMG_H
#define TUTORIAL_PMG_H

#include "display.h"

#define PLAYER(n) (PMRAM + 0x400 + (word)(n) * 256)
#define HPOS(n) REG8(0xd000 + (n))
#define PCOLOR(n) REG8(0x02c0 + (n))
#define HITCLR REG8(0xd01e)

static void pmg_on(void)
{
    clear_dma(PMRAM, 0x800);
    REG8(0xd407) = 0x90;
    REG8(0xd01d) = 2;
    REG8(0x026f) = 1;
    for (byte i = 0; i < 4; i++)
    {
        HPOS(i) = 0;
        REG8(0xd008 + i) = 0;
        PCOLOR(i) = 0x0e;
    }
    SDMCTL |= 0x18;
}

static void player_draw(byte slot, byte x, byte y, const byte *shape, byte height)
{
    copy_dma(PLAYER(slot) + y, shape, height);
    HPOS(slot) = x;
}

static void player_erase(byte slot, byte y, byte height)
{
    clear_dma(PLAYER(slot) + y, height);
}

#endif

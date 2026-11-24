#ifndef TUTORIAL_BITMAP_H
#define TUTORIAL_BITMAP_H

#include <stdlib.h>
#include "display.h"

enum DrawMode { SET_PIXELS, XOR_PIXELS, CLEAR_PIXELS };

static void pixel(volatile byte *base, int x, int y, DrawMode mode)
{
    static const byte masks[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    if (x < 0 || x >= 320 || y < 0 || y >= 192)
        return;
    volatile byte *p = bitmap_row(base, y) + (x >> 3);
    byte bit = masks[x & 7];
    if (mode == SET_PIXELS)
        *p |= bit;
    else if (mode == XOR_PIXELS)
        *p ^= bit;
    else
        *p &= ~bit;
}

static void swap_bitmap(volatile byte *next)
{
    wait_blank();
    DL[5] = (word)next >> 8;
    DL[103] = ((word)next + 0x1000) >> 8;
}

#endif

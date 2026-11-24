#ifndef TUTORIAL_TILEMAP_H
#define TUTORIAL_TILEMAP_H

#include "scroll.h"

// Define map_tiles, tile_map and MAP_WIDTH before including this header.
// Tiles are four characters across and four characters high.
static void tile_row(volatile byte *dst, byte count, byte left, byte top)
{
    const byte *map = tile_map + (word)(top >> 2) * MAP_WIDTH;
    for (byte x = 0; x < count; x++, left++)
    {
        byte tile = map[left >> 2];
        dst[x] = map_tiles[16 * (word)tile + 4 * (top & 3) + (left & 3)];
    }
}

static void tile_window(volatile byte *dst, byte pitch, byte height, byte left, byte top)
{
    for (byte y = 0; y < height; y++, dst += pitch)
        tile_row(dst, pitch, left, top + y);
}

static void tile_display(void)
{
    text_display(4, true);
    COLOR(0) = 0;
    COLOR(1) = 0x06;
    COLOR(2) = 0x8c;
    COLOR(3) = 0xbc;
    COLOR(4) = 0x04;
}

#endif

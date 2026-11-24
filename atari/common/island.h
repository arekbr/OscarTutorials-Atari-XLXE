#ifndef TUTORIAL_ISLAND_H
#define TUTORIAL_ISLAND_H

#include "scroll.h"

// The first eight CharPad glyphs encode two 2x2 backgrounds. Moving
// their pixels independently of ANTIC fine scroll supplies parallax.
static byte background[64];

static void island_begin(void)
{
    for (byte i = 0; i < sizeof(background); i++)
        background[i] = FONT[i];
    text_display(4, true);
    COLOR(0) = 0;
    COLOR(1) = 0x06;
    COLOR(2) = 0x0e;
    COLOR(4) = 0x84;
}

static void parallax(byte phase_x, byte phase_y)
{
    byte shift = 2 * (phase_x & 3);
    byte side = (phase_x >> 2) & 1;
    for (byte group = 0; group < 64; group += 32)
        for (byte row = 0; row < 16; row++)
        {
            byte y = (row + phase_y) & 15;
            word pixels = (word)background[group + y] << 8 | background[group + 16 + y];
            if (shift)
                pixels = (pixels << shift) | (pixels >> (16 - shift));
            FONT[group + row] = side ? (byte)pixels : (byte)(pixels >> 8);
            FONT[group + 16 + row] = side ? (byte)(pixels >> 8) : (byte)pixels;
        }
}

#endif

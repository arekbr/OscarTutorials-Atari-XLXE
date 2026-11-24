#ifndef TUTORIAL_SHAPE_H
#define TUTORIAL_SHAPE_H

#include "pmg.h"

// Convert the 24-pixel VIC shape to eight GTIA colour clocks. A group
// of three source pixels lights one GTIA bit if any of them was set.
static void player_shape(const byte *source, byte *target)
{
    for (byte y = 0; y < 21; y++)
    {
        unsigned long bits = (unsigned long)source[0] << 16 |
                             (word)source[1] << 8 | source[2];
        byte output = 0;
        for (byte x = 0; x < 8; x++)
        {
            output <<= 1;
            if (bits & 0xe00000UL)
                output |= 1;
            bits <<= 3;
        }
        target[y] = output;
        source += 3;
    }
}

static const byte player_colors[] = { 0x0e, 0x3a, 0x7c, 0xca };

static void sprite_stage(void)
{
    clear_dma(TEXT, 960);
    text_display(2, false);
    pmg_on();
}

#endif

#ifndef TUTORIAL_DISPLAY_H
#define TUTORIAL_DISPLAY_H

#include "hw.h"

// The display list occupies $5c00-$5fff. Keep ANTIC buffers outside C's
// arena and begin a new LMS before crossing each 4 KB DMA boundary.
#pragma region(main, 0x2080, 0x5c00, , , {code, data, bss, heap, stack})

#define DL ((volatile byte *)0x5c00)
#define ALT_DL ((volatile byte *)0x5d00)
#define TEXT ((volatile byte *)0x8000)
#define BACK_TEXT ((volatile byte *)0x8400)
#define FONT ((volatile byte *)0x8800)
#define ROM_FONT ((const byte *)0xe000)
#define PMRAM ((volatile byte *)0x9000)
#define BITMAP0 ((volatile byte *)0x6000)
#define BITMAP1 ((volatile byte *)0x8000)
#define TV_ROWS 24
#define TV_PITCH 40

static byte list_pos;

static void list_begin(void)
{
    list_pos = 0;
    DL[list_pos++] = 0x70;
    DL[list_pos++] = 0x70;
    DL[list_pos++] = 0x70;
}

static void list_address(word address)
{
    DL[list_pos++] = (byte)address;
    DL[list_pos++] = address >> 8;
}

static void list_line(byte mode, const volatile byte *row)
{
    DL[list_pos++] = mode | 0x40;
    list_address((word)row);
}

static void list_end(void)
{
    DL[list_pos++] = 0x41;
    list_address((word)DL);
}

static void display_on(byte dma)
{
    REG16(0x0230) = (word)DL;
    SDMCTL = dma;
    wait_frame();
}

static void display_off(void)
{
    NMIEN = 0x40;
    SDMCTL = 0;
    DMACTL = 0;
}

static void text_list(byte mode, const volatile byte *base)
{
    list_begin();
    list_line(mode, base);
    for (byte y = 1; y < TV_ROWS; y++)
        DL[list_pos++] = mode;
    list_end();
}

static void text_display(byte mode, bool custom_font)
{
    display_off();
    CHBAS = custom_font ? 0x88 : 0xe0;
    COLOR(0) = 0x2a;
    COLOR(1) = 0x0e;
    COLOR(2) = mode == 2 ? 0x80 : 0x94;
    COLOR(3) = 0x46;
    COLOR(4) = 0;
    text_list(mode, TEXT);
    display_on(0x22);
}

static void clear_dma(volatile byte *target, word size)
{
    while (size--)
        *target++ = 0;
}

static void copy_dma(volatile byte *target, const byte *source, word size)
{
    while (size--)
        *target++ = *source++;
}

static void bitmap_list(byte mode, const volatile byte *base, byte height)
{
    list_begin();
    list_line(mode, base);
    for (byte y = 1; y < height; y++)
    {
        if (y == 96)
            list_line(mode, base + 0x1000);
        else
            DL[list_pos++] = mode;
    }
    list_end();
}

static volatile byte *bitmap_row(volatile byte *base, byte row)
{
    if (row < 96)
        return base + (word)row * 40;
    return base + 0x1000 + (word)(row - 96) * 40;
}

static void bitmap_display(byte mode, volatile byte *base)
{
    display_off();
    COLOR(0) = 0x2a;
    COLOR(1) = 0x0e;
    COLOR(2) = mode == 15 ? 0x00 : 0x94;
    COLOR(4) = 0;
    bitmap_list(mode, base, 192);
    display_on(0x22);
}

#endif

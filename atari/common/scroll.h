#ifndef TUTORIAL_SCROLL_H
#define TUTORIAL_SCROLL_H

#include "display.h"

#define SCROLL_PITCH 48
#define SCROLL_ROWS 25
#define SCROLL_FRONT ((volatile byte *)0x6000)
#define SCROLL_BACK ((volatile byte *)0x6800)

static volatile byte *scroll_front = SCROLL_FRONT;
static volatile byte *scroll_back = SCROLL_BACK;
static volatile byte *visible_list = DL;

static void select_list(volatile byte *next)
{
    REG16(0x0230) = (word)next;
    REG16(0xd402) = (word)next;
    visible_list = next;
}

// Each LMS supplies 48 bytes for a normal-width HSCROL row. For
// vertical scroll the first 24 rows form a region and row 25 ends it.
static volatile byte *prepare_scroll(const volatile byte *screen, byte mode, byte pitch,
                                     bool horizontal, bool vertical)
{
    volatile byte *list = visible_list == DL ? ALT_DL : DL;
    volatile byte *p = list;
    *p++ = 0x70;
    *p++ = 0x70;
    *p++ = 0x70;
    if (vertical)
        p[-1] = 0x60; // 23 blank lines + 193 scrolled lines = 216
    byte command = mode | 0x40;
    if (horizontal)
        command |= 0x10;
    for (byte y = 0; y < (vertical ? 25 : 24); y++)
    {
        *p++ = command | ((vertical && y < 24) ? 0x20 : 0);
        *p++ = (byte)(word)screen;
        *p++ = (word)screen >> 8;
        screen += pitch;
    }
    *p++ = 0x41;
    *p++ = (byte)(word)list;
    *p = (word)list >> 8;
    return list;
}

static void publish_scroll(volatile byte *next, byte h, byte v)
{
    wait_blank();
    HSCROL = h;
    VSCROL = v;
    select_list(next);
}

static void scroll_flip(void)
{
    volatile byte *old = scroll_front;
    scroll_front = scroll_back;
    scroll_back = old;
}

static byte diagonal(void)
{
    return (REG8(0xd20a) & 1) + screen_code(6);
}

#endif

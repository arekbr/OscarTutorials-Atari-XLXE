#include "../common/scroll.h"

// Each row contains a repeating 16-character strip plus ANTIC's
// extra fetch area. LMS moves through it by one byte every 4 clocks.
#define MAP ((volatile byte *)0x6000)
#define PITCH 64

static volatile byte *make_list(byte sx, byte sy)
{
    volatile byte *list = visible_list == DL ? ALT_DL : DL;
    volatile byte *p = list;
    *p++ = 0x70;
    *p++ = 0x70;
    *p++ = 0x70;
    for (byte y = 0; y < 24; y++)
    {
        word address = (word)MAP + (word)((sy + y) & 31) * PITCH + sx;
        *p++ = 0x52;
        *p++ = (byte)address;
        *p++ = address >> 8;
    }
    *p++ = 0x41;
    *p++ = (byte)(word)list;
    *p = (word)list >> 8;
    return list;
}

int main(void)
{
    for (byte y = 0; y < 32; y++)
    {
        for (byte x = 0; x < 16; x++)
            MAP[(word)y * PITCH + x] = diagonal();
        for (byte x = 16; x < PITCH; x++)
            MAP[(word)y * PITCH + x] = MAP[(word)y * PITCH + (x & 15)];
    }

    byte px = 0, py = 0;
    byte old_x = 255, old_y = 255;
    volatile byte *next = DL;
    text_display(2, false);
    for (;;)
    {
        px--;
        byte x = (px >> 2) & 15;
        byte y = py >> 3;
        if (x != old_x || y != old_y)
            next = make_list(x, y);
        publish_scroll(next, 12 - (px & 3), py & 7);
        old_x = x;
        old_y = y;
    }
}

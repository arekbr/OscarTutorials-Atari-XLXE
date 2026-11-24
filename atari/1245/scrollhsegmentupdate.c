#include "../common/scroll.h"

#define LEVEL ((volatile byte *)0x7000)
#define LEVEL_PITCH 64

static void copy_segment(byte first, byte column)
{
    for (byte y = first; y < first + 6; y++)
    {
        volatile byte *dst = scroll_back + (word)y * SCROLL_PITCH;
        const volatile byte *src = LEVEL + (word)y * LEVEL_PITCH;
        for (byte x = 0; x < SCROLL_PITCH; x++)
            dst[x] = src[(column + x) & 63];
    }
}

int main(void)
{
    for (byte y = 0; y < 24; y++)
        for (byte x = 0; x < LEVEL_PITCH; x++)
            LEVEL[(word)y * LEVEL_PITCH + x] = diagonal();

    for (byte y = 0; y < 24; y += 6)
        copy_segment(y, 0);
    scroll_flip();
    text_display(2, false);
    byte column = 0;
    byte part = 0;
    signed char direction = 1;
    volatile byte *next = prepare_scroll(scroll_front, 2, SCROLL_PITCH, true, false);
    publish_scroll(next, 12, 0);

    for (;;)
    {
        // Prepare one quarter of the adjacent viewport per fine step.
        // Direction changes wait for a cell boundary, so a partially
        // prepared page can never go on display.
        if (!part && joy_x())
            direction = joy_x();
        byte pending = (column + direction) & 63;
        copy_segment(part * 6, pending);
        part++;
        if (part == 4)
        {
            scroll_flip();
            next = prepare_scroll(scroll_front, 2, SCROLL_PITCH, true, false);
            column = pending;
            part = 0;
        }
        byte fine = direction > 0 ? 12 - part : 12 + part;
        publish_scroll(next, fine, 0);
    }
}

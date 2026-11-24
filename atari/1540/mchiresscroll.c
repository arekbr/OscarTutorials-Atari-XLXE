#include "../common/scroll.h"

// Five 96-line strips, each in its own 4 KB ANTIC DMA bank.
#pragma section(image, 0)
#pragma region(image, 0x6000, 0xb000, , , {image})
#pragma data(image)
static const byte landscape[] = {
    #embed "../assets/bitmap-scroll.antic"
};
#pragma data(data)

static const byte palette[] = {
    #embed "../assets/bitmap-scroll.colors"
};

static volatile byte *picture(byte block, byte line)
{
    volatile byte *list = visible_list == DL ? ALT_DL : DL;
    volatile byte *p = list;
    *p++ = 0x70;
    *p++ = 0x70;
    *p++ = 0x70;
    for (byte y = 0; y < 192; y++)
    {
        if (!y || !line)
        {
            word address = (word)landscape + (word)block * 4096 + (word)line * 40;
            *p++ = 0x4e;
            *p++ = (byte)address;
            *p++ = address >> 8;
        }
        else
            *p++ = 14;
        if (++line == 96)
        {
            line = 0;
            block++;
        }
    }
    *p++ = 0x41;
    *p++ = (byte)(word)list;
    *p = (word)list >> 8;
    return list;
}

int main(void)
{
    display_off();
    COLOR(4) = palette[0];
    COLOR(0) = palette[1];
    COLOR(1) = palette[2];
    COLOR(2) = palette[3];
    word y = 0;
    int direction = 1;
    for (;;)
    {
        volatile byte *next = picture(y / 96, y % 96);
        publish_scroll(next, 0, 0);
        SDMCTL = 0x22;
        if (y == 288)
            direction = -1;
        else if (!y)
            direction = 1;
        y += direction;
    }
}

#include "../common/bands.h"

#define STRIPS ((volatile byte *)0x6000)
#define STRIP_PITCH 64

int main(void)
{
    clear_dma(TEXT, 960);
    write_text(TEXT + 40 * 11 + 8, "STATIONARY PLAYFIELD");
    for (byte y = 0; y < 24; y++)
        for (byte x = 0; x < STRIP_PITCH; x++)
            STRIPS[(word)y * STRIP_PITCH + x] = ((x & 15) + y) & 3 ? screen_code(6 + ((x ^ y) & 1)) : 0;

    display_off();
    list_begin();
    DL[2] = 0xf0;
    for (byte y = 0; y < 24; y++)
    {
        if (y == 11 || y == 12)
            list_line(y == 12 ? 0x82 : 2, TEXT + 40 * y);
        else
            list_line(y == 10 ? 0x92 : 0x12, STRIPS + (word)y * STRIP_PITCH);
    }
    list_end();
    band_hue[0] = 0x82;
    band_hue[1] = 0x42;
    band_hue[2] = 0xa4;
    bands_on(3);

    byte phase = 0;
    for (;;)
    {
        wait_blank();
        byte left = phase++;
        byte right = -left;
        band_fine[0] = 12 - (left & 3);
        band_fine[1] = 0;
        band_fine[2] = 12 - (right & 3);
        for (byte y = 0; y < 24; y++)
        {
            if (y == 11 || y == 12)
                continue;
            // Both surfaces remain within their 64-byte LMS rows.
            DL[4 + 3 * y] = (byte)((word)y * STRIP_PITCH) +
                            (((y < 11 ? left : right) >> 2) & 15);
        }
    }
}

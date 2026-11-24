#include "../common/display.h"

int main(void)
{
    // 01, 10 and 11 select the playfield colours. ANTIC 4 uses
    // screen bit 7 to select PF3 instead of PF2 for 11 pixels.
    static const byte bars[] = { 0, 1, 2, 3, 0x83 };
    for (byte i = 0; i < 32; i++)
        FONT[i] = (i >> 3) * 0x55;
    for (byte y = 0; y < 24; y++)
        for (byte x = 0; x < 40; x++)
            TEXT[40 * y + x] = bars[x >> 3];
    text_display(4, true);
    for (;;) {}
}

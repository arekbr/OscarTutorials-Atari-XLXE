#include "../common/scroll.h"

int main(void)
{
    clear_dma(TEXT, 960);
    text_display(2, false);
    for (;;)
    {
        wait_blank();
        COLOR(4) = 0x32;
        COLBK = 0x32;
        for (byte x = 0; x < 40; x++)
        {
            #pragma unroll(full)
            for (byte y = 0; y < 12; y++)
                TEXT[40 * y + x] = TEXT[40 * (y + 1) + x];
        }
        for (byte x = 0; x < 40; x++)
        {
            #pragma unroll(full)
            for (byte y = 12; y < 23; y++)
                TEXT[40 * y + x] = TEXT[40 * (y + 1) + x];
        }
        COLBK = 0x02;
        for (byte x = 0; x < 40; x++)
            TEXT[920 + x] = diagonal();
        COLOR(4) = 0;
    }
}

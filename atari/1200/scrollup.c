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
        for (word i = 0; i < 920; i++)
            TEXT[i] = TEXT[i + 40];
        COLBK = 0x02;
        for (byte x = 0; x < 40; x++)
            TEXT[920 + x] = diagonal();
        COLOR(4) = 0;
    }
}

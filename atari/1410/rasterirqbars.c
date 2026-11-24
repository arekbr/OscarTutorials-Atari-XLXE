#include "../common/raster.h"

int main(void)
{
    static const char message[] = "HELLO WORLD ";
    clear_dma(TEXT, 960);
    text_display(2, false);
    wait_blank();

    for (byte i = 0; i < 16; i++)
    {
        raster_border[i] = 0x80 | i * 2;
        raster_background[i] = 0xa0 | i * 2;
        DL[6 + 4 + i] |= 0x80;
    }
    start_color_dli(16);

    // The main loop writes characters while ANTIC interrupts it.
    word cursor = 0;
    for (;;)
    {
        write_text(TEXT + cursor, message);
        cursor += sizeof(message) - 1;
        if (cursor >= 960)
            cursor = 0;
        wait_frame();
    }
}

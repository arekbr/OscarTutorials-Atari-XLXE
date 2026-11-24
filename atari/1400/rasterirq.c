#include "../common/raster.h"

int main(void)
{
    static const char message[] = "HELLO WORLD ";
    clear_dma(TEXT, 960);
    text_display(2, false);
    wait_blank();

    raster_border[0] = 0x84;
    raster_border[1] = 0x9a;
    raster_background[0] = 0xa6;
    raster_background[1] = 0x74;
    DL[6 + 6] |= 0x80;
    DL[6 + 13] |= 0x80;
    start_color_dli(2);

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

#include <math.h>
#include "../common/raster.h"

int main(void)
{
    byte position[64];
    for (byte i = 0; i < 64; i++)
        position[i] = 9 + (int)(7 * sin(i * PI / 32));

    clear_dma(TEXT, 960);
    text_display(2, false);
    raster_border[0] = 0x36;
    raster_border[1] = 0x94;
    raster_background[0] = 0x8a;
    raster_background[1] = 0x80;

    byte phase = 0, row = position[0];
    wait_blank();
    DL[6 + row] |= 0x80;
    DL[10 + row] |= 0x80;
    start_color_dli(2);
    for (;;)
    {
        wait_blank();
        DL[6 + row] &= 0x7f;
        DL[10 + row] &= 0x7f;
        phase = (phase + 1) & 63;
        row = position[phase];
        DL[6 + row] |= 0x80;
        DL[10 + row] |= 0x80;
    }
}

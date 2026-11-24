#include "../common/vector.h"

template<int Address>
void clear_sheet(void)
{
    // Absolute indexed writes cover the same offset in every page.
    for (word x = 0; x < 256; x++)
    {
        #pragma unroll(full)
        for (byte page = 0; page < 32; page++)
            ((volatile byte *)(word)Address)[256 * page + x] = 0;
    }
}

int main(void)
{
    clear_dma(BITMAP0, 8192);
    clear_dma(BITMAP1, 8192);
    bitmap_display(15, BITMAP0);

    Point star[10];
    for (byte phase = 0;; phase++)
    {
        volatile byte *back = (phase & 1) ? BITMAP0 : BITMAP1;
        if (phase & 1)
            clear_sheet<0x6000>();
        else
            clear_sheet<0x8000>();
        star_points(star, phase, phase);
        star_draw(back, star, SET_PIXELS);
        swap_bitmap(back);
    }
}

#include "../common/vector.h"

int main(void)
{
    clear_dma(BITMAP0, 8192);
    clear_dma(BITMAP1, 8192);
    bitmap_display(15, BITMAP0);

    Point stars[2][10];
    byte count = 0;
    for (byte phase = 0;; phase++)
    {
        byte slot = phase & 1;
        volatile byte *back = slot ? BITMAP0 : BITMAP1;
        if (count >= 2)
            star_draw(back, stars[slot], CLEAR_PIXELS);
        else
            count++;
        star_points(stars[slot], phase, phase);
        star_draw(back, stars[slot], SET_PIXELS);
        swap_bitmap(back);
    }
}

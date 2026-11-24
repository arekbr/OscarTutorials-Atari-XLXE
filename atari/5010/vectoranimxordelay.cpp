#include "../common/vector.h"

int main(void)
{
    clear_dma(BITMAP0, 8192);
    bitmap_display(15, BITMAP0);

    Point stars[2][10];
    bool drawn = false;
    for (byte phase = 0;; phase++)
    {
        byte slot = phase & 1;
        star_points(stars[slot], phase, phase);
        star_draw(BITMAP0, stars[slot], XOR_PIXELS);
        if (drawn)
            star_draw(BITMAP0, stars[slot ^ 1], XOR_PIXELS);
        wait_blank();
        drawn = true;
    }
}

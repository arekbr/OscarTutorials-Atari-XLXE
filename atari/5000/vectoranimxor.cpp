#include "../common/vector.h"

int main(void)
{
    clear_dma(BITMAP0, 8192);
    bitmap_display(15, BITMAP0);

    Point star[10];
    for (byte phase = 0;; phase++)
    {
        star_points(star, phase, phase);
        star_draw(BITMAP0, star, XOR_PIXELS);
        wait_blank();
        star_draw(BITMAP0, star, XOR_PIXELS);
    }
}

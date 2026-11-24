#include "../common/island.h"

static const byte charset[] = {
    #embed ctm_chars "../../Resources/metalisland.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/metalisland.ctm"
};

static void island_window(volatile byte *target, byte x, byte y)
{
    const byte *source = tile_map + (word)y * 80 + x;
    for (byte row = 0; row < 25; row++)
    {
        copy_dma(target, source, SCROLL_PITCH);
        target += SCROLL_PITCH;
        source += 80;
    }
}

int main(void)
{
    copy_dma(FONT, charset, sizeof(charset));
    clear_dma(TEXT, 960);
    island_begin();

    int px = 0, py = 0;
    int vx = 0, vy = 0;
    byte old_x = 255, old_y = 255;
    volatile byte *next = DL;
    for (;;)
    {
        vx += 5 * joy_x() - ((vx + 2) >> 2);
        vy += 5 * joy_y() - ((vy + 2) >> 2);
        px += vx;
        py += vy;
        if (px < 0 || px > 32 * 4 * 8)
        {
            px -= vx;
            vx = -vx;
        }
        if (py < 0 || py > 25 * 8 * 8)
        {
            py -= vy;
            vy = -vy;
        }
        byte x = px >> 5, y = py >> 6;
        if (x != old_x || y != old_y)
        {
            island_window(scroll_back, x, y);
            next = prepare_scroll(scroll_back, 4, SCROLL_PITCH, true, true);
            scroll_flip();
            old_x = x;
            old_y = y;
        }
        publish_scroll(next, 12 - ((px >> 3) & 3), (py >> 3) & 7);
        parallax(7 - ((px >> 4) & 7), 15 - ((py >> 4) & 15));
    }
}

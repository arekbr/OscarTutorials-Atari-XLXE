#include "../common/display.h"
#include "../common/navigation.h"
#define MAP_WIDTH 20

static const byte charset[] = {
    #embed ctm_chars "../../Resources/petilebitmapmc.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/petilebitmapmc.ctm"
};
static const byte map_tiles[] = {
    #embed ctm_tiles8 "../../Resources/petilebitmapmc.ctm"
};
static const byte attributes1[] = {
    #embed ctm_attr1 "../../Resources/petilebitmapmc.ctm"
};
static const byte attributes2[] = {
    #embed ctm_attr2 "../../Resources/petilebitmapmc.ctm"
};

// Collapse the Commodore palette to black, two colours and a highlight.
static const byte ink[] = { 0, 3, 1, 2, 1, 2, 1, 3, 1, 0, 1, 0, 2, 3, 2, 3 };
static byte converted[sizeof(map_tiles)][8];

static void prepare_tiles(void)
{
    for (byte tile = 0; tile < sizeof(map_tiles) / 16; tile++)
    {
        byte colors[4] = { 0, ink[attributes2[tile] >> 4],
                            ink[attributes2[tile] & 15], ink[attributes1[tile] & 15] };
        for (byte c = 0; c < 16; c++)
        {
            const byte *src = charset + 8 * map_tiles[16 * tile + c];
            for (byte y = 0; y < 8; y++)
            {
                byte old = src[y], pixels = 0;
                for (byte x = 0; x < 4; x++)
                {
                    pixels = (pixels << 2) | colors[old >> 6];
                    old <<= 2;
                }
                converted[16 * tile + c][y] = pixels;
            }
        }
    }
}

static void show(volatile byte *base, byte left, byte top)
{
    for (byte y = 0; y < 24; y++)
    {
        const byte *map = tile_map + (word)(top + (y >> 2)) * MAP_WIDTH + left;
        for (byte scan = 0; scan < 8; scan++)
        {
            volatile byte *dst = bitmap_row(base, 8 * y + scan);
            for (byte x = 0; x < 40; x++)
                dst[x] = converted[16 * map[x >> 2] + 4 * (y & 3) + (x & 3)][scan];
        }
    }
}

int main(void)
{
    byte x = 0, y = 0;
    volatile byte *front = BITMAP0;
    volatile byte *back = BITMAP1;
    prepare_tiles();
    show(front, x, y);
    bitmap_display(14, front);
    COLOR(0) = 0x38;
    COLOR(1) = 0x9a;
    COLOR(2) = 0x0e;

    for (;;)
    {
        byte direction;
        do
        {
            wait_frame();
            direction = navigation();
        } while (!direction);
        signed char dx = navigation_x(direction);
        signed char dy = navigation_y(direction);
        if ((dx < 0 && x) || (dx > 0 && x < 10)) x += dx;
        if ((dy < 0 && y) || (dy > 0 && y < 14)) y += dy;

        show(back, x, y);
        wait_blank();
        // Only two LMS high bytes change. Both buffers are page-aligned.
        DL[5] = (word)back >> 8;
        DL[103] = ((word)back + 0x1000) >> 8;
        volatile byte *old = front;
        front = back;
        back = old;
    }
}

#include "../common/scroll.h"
#include "../common/navigation.h"
#define MAP_WIDTH 30
#define MAP_HEIGHT MAP_WIDTH

static const byte charset[] = {
    #embed ctm_chars "../../Resources/pipes30.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/pipes30.ctm"
};
static const byte map_tiles[] = {
    #embed ctm_tiles8 "../../Resources/pipes30.ctm"
};
#include "../common/tilemap.h"

// Retile as [subrow][tile][x]. Four consecutive cells share the same
// pointer and tile lookup, including the middle of an unaligned view.
static byte retiled[sizeof(map_tiles)];

static void retile(void)
{
    for (byte tile = 0; tile < sizeof(map_tiles) / 16; tile++)
        for (byte y = 0; y < 4; y++)
            for (byte x = 0; x < 4; x++)
                retiled[(sizeof(map_tiles) / 4) * y + 4 * tile + x] =
                    map_tiles[16 * tile + 4 * y + x];
}

static void fast_window(volatile byte *dst, byte left, byte top)
{
    for (byte y = 0; y < 25; y++)
    {
        byte wy = top + y;
        const byte *map = tile_map + (word)(wy >> 2) * MAP_WIDTH + (left >> 2);
        const byte *row = retiled + (sizeof(map_tiles) / 4) * (wy & 3);
        byte x = left & 3;
        byte copied = 0;
        while (copied < SCROLL_PITCH)
        {
            const byte *src = row + *map++ * 4;
            do {
                *dst++ = src[x++];
                copied++;
            } while (x < 4 && copied < SCROLL_PITCH);
            x = 0;
        }
    }
}

int main(void)
{
    copy_dma(FONT, charset, sizeof(charset));
    clear_dma(TEXT, 960);
    tile_display();
    int px = 1, py = 1;
    signed char dx = 1, dy = 1;
    retile();
    byte old_x = 255, old_y = 255;
    volatile byte *next = DL;
    for (;;)
    {
        byte direction = navigation();
        signed char nx = dx + navigation_x(direction);
        signed char ny = dy + navigation_y(direction);
        if (nx >= -4 && nx <= 4) dx = nx;
        if (ny >= -4 && ny <= 4) dy = ny;

        const int max_x = (MAP_WIDTH * 4 - 48) * 4;
        const int max_y = (MAP_HEIGHT * 4 - 25) * 8;
        px += dx;
        py += dy;
        if (px < 0 || px > max_x)
        {
            px = px < 0 ? 0 : max_x;
            dx = -dx;
        }
        if (py < 0 || py > max_y)
        {
            py = py < 0 ? 0 : max_y;
            dy = -dy;
        }
        byte x = px >> 2, y = py >> 3;
        if (x != old_x || y != old_y)
        {
            fast_window(scroll_back, x, y);
            next = prepare_scroll(scroll_back, 4, SCROLL_PITCH, true, true);
            scroll_flip();
            old_x = x;
            old_y = y;
        }
        publish_scroll(next, 12 - (px & 3), py & 7);
    }
}

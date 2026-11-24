#include "../common/scroll.h"
#include "../common/navigation.h"
#define MAP_WIDTH 20

static const byte charset[] = {
    #embed ctm_chars "../../Resources/pertilecolor.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/pertilecolor.ctm"
};
static const byte map_tiles[] = {
    #embed ctm_tiles8 "../../Resources/pertilecolor.ctm"
};
static const byte attributes1[] = {
    #embed ctm_attr1 "../../Resources/pertilecolor.ctm"
};

static byte colored_tiles[sizeof(map_tiles)];

static void prepare_tiles(void)
{
    for (word i = 0; i < sizeof(map_tiles); i++)
    {
        byte glyph = map_tiles[i];
        byte ink = attributes1[i >> 4] & 15;
        // Screen bit 7 chooses an alternate colour for pixels with value 11.
        colored_tiles[i] = glyph | ((0x6c80U & (1U << ink)) ? 0x80 : 0);
    }
}

static void show(byte left, byte top)
{
    volatile byte *screen = scroll_back;
    for (byte y = 0; y < 24; y++)
    {
        const byte *map = tile_map + (word)(top + (y >> 2)) * MAP_WIDTH + left;
        byte subrow = 4 * (y & 3);
        for (byte x = 0; x < 10; x++)
        {
            word index = (word)map[x] * 16 + subrow;
            copy_dma(screen, colored_tiles + index, 4);
            screen += 4;
        }
    }
}

int main(void)
{
    prepare_tiles();
    copy_dma(FONT, charset, sizeof(charset));
    clear_dma(TEXT, 960);
    text_display(4, true);
    COLOR(0) = 0x02;
    COLOR(1) = 0x0e;
    COLOR(2) = 0x36;
    COLOR(3) = 0x9c;
    COLOR(4) = 0x04;
    byte x = 0, y = 0;
    for (;;)
    {
        show(x, y);
        volatile byte *next = prepare_scroll(scroll_back, 4, 40, false, false);
        publish_scroll(next, 0, 0);
        scroll_flip();
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
    }
}

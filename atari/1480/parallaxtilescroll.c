#include "../common/island.h"
#include "../common/shape.h"
#define MAP_WIDTH 16

static const byte charset[] = {
    #embed ctm_chars "../../Resources/metalisletiles.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/metalisletiles.ctm"
};
static const byte map_tiles[] = {
    #embed ctm_tiles8 "../../Resources/metalisletiles.ctm"
};
#include "../common/tilemap.h"

static const byte ships[] = {
    #embed spd_sprites "../../Resources/vship.spd"
};

int main(void)
{
    byte shape[3][21];
    for (byte frame = 0; frame < 3; frame++)
        player_shape(ships + 64 * frame, shape[frame]);

    copy_dma(FONT, charset, sizeof(charset));
    clear_dma(TEXT, 960);
    island_begin();
    pmg_on();
    PCOLOR(0) = 0x2c;

    byte ship_x = 124, ship_y = 176, last_y = 0;
    word py = 1536;
    byte old_x = 255, old_y = 255;
    volatile byte *next = DL;
    for (;;)
    {
        signed char horizontal = joy_x();
        int sx = ship_x + horizontal, sy = ship_y + joy_y();
        if (sx >= 52 && sx <= 187) ship_x = sx;
        if (sy >= 144 && sy <= 193) ship_y = sy;
        byte px = (ship_x - 52) >> 1;
        py = (py - 1) & 2047;
        byte x = px >> 2, y = py >> 3;
        if (x != old_x || y != old_y)
        {
            for (byte row = 0; row < 25; row++)
                tile_row(scroll_back + (word)row * SCROLL_PITCH, SCROLL_PITCH, x, y + row);
            next = prepare_scroll(scroll_back, 4, SCROLL_PITCH, true, true);
            scroll_flip();
            old_x = x;
            old_y = y;
        }
        publish_scroll(next, 12 - (px & 3), py & 7);
        parallax(7 - ((px >> 1) & 7), 15 - ((py >> 1) & 15));
        player_erase(0, last_y, 21);
        player_draw(0, ship_x, ship_y, shape[1 + horizontal], 21);
        last_y = ship_y;
    }
}

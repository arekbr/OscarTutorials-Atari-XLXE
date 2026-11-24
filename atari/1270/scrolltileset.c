#include "../common/scroll.h"
#include "../common/navigation.h"
#define MAP_WIDTH 16
#define MAP_HEIGHT MAP_WIDTH

static const byte charset[] = {
    #embed ctm_chars "../../Resources/pipes.ctm"
};
static const byte tile_map[] = {
    #embed ctm_map8 "../../Resources/pipes.ctm"
};
static const byte map_tiles[] = {
    #embed ctm_tiles8 "../../Resources/pipes.ctm"
};
#include "../common/tilemap.h"

int main(void)
{
    copy_dma(FONT, charset, sizeof(charset));
    clear_dma(TEXT, 960);
    tile_display();
    int px = 0, py = 0;
    byte direction = 0;

    byte old_x = 255, old_y = 255;
    volatile byte *next = DL;
    for (;;)
    {
        byte input = navigation();
        if (input)
            direction = input;
        int nx = px + navigation_x(direction);
        int ny = py + navigation_y(direction);
        if (nx >= 0 && nx <= (MAP_WIDTH * 4 - 48) * 4) px = nx;
        if (ny >= 0 && ny <= (MAP_HEIGHT * 4 - 25) * 8) py = ny;
        byte x = px >> 2, y = py >> 3;
        if (x != old_x || y != old_y)
        {
            tile_window(scroll_back, SCROLL_PITCH, 25, x, y);
            next = prepare_scroll(scroll_back, 4, SCROLL_PITCH, true, true);
            scroll_flip();
            old_x = x;
            old_y = y;
        }
        publish_scroll(next, 12 - (px & 3), py & 7);
    }
}

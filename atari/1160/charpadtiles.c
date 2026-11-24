#include "../common/scroll.h"
#include "../common/navigation.h"
#define MAP_WIDTH 16

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

    byte x = 0, y = 0;
    for (;;)
    {
        wait_blank();
        tile_window(TEXT, 40, 24, x, y);
        byte direction;
        do
        {
            wait_frame();
            direction = navigation();
        } while (!direction);
        signed char dx = navigation_x(direction);
        signed char dy = navigation_y(direction);
        if ((dx < 0 && x) || (dx > 0 && x < 24)) x += 4 * dx;
        if ((dy < 0 && y) || (dy > 0 && y < 40)) y += 4 * dy;
        for (byte delay = 0; delay < 7; delay++) wait_frame();
    }
}

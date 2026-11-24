#include "../common/pmg.h"

#define WIDTH 4
#define HEIGHT 8
#define WALL 1

static const byte shape[HEIGHT] = { 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0 };

int main(void)
{
    clear_dma(TEXT, 960);
    clear_dma(FONT, 32);
    for (byte y = 0; y < 8; y++)
        FONT[8 + y] = 0x55;
    for (byte y = 1; y < 23; y++)
        for (byte x = 1; x < 39; x++)
            if ((x < 18 || x > 22 || y < 10 || y > 14) && REG8(0xd20a) < 25)
                TEXT[40 * y + x] = WALL;

    text_display(4, true);
    pmg_on();
    COLOR(0) = 0x94;
    byte x = 76, y = 88, last_y = 0;
    for (;;)
    {
        wait_blank();
        int nx = x + joy_x(), ny = y + joy_y();
        if (nx < 0 || nx > 160 - WIDTH) nx = x;
        if (ny < 0 || ny > 192 - HEIGHT) ny = y;

        x = nx;
        y = ny;
        // Player 0 vs the four ANTIC playfield priorities (latched).
        PCOLOR(0) = REG8(0xd004) ? 0x38 : 0x0e;
        HITCLR = 0;
        player_erase(0, last_y, HEIGHT);
        last_y = 32 + y;
        player_draw(0, 48 + x, last_y, shape, HEIGHT);
    }
}

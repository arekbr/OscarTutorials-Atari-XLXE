#include "../common/pmg.h"

#define WIDTH 4
#define HEIGHT 8
#define WALL 1

static const byte shape[HEIGHT] = { 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0 };

static bool wall_at(byte x, byte y)
{
    return TEXT[40 * (y >> 3) + (x >> 2)] != 0;
}

static bool blocked(byte x, byte y)
{
    return wall_at(x, y) || wall_at(x + WIDTH - 1, y) ||
           wall_at(x, y + HEIGHT - 1) || wall_at(x + WIDTH - 1, y + HEIGHT - 1);
}

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

        if (!blocked(nx, y)) x = nx;
        if (!blocked(x, ny)) y = ny;
        player_erase(0, last_y, HEIGHT);
        last_y = 32 + y;
        player_draw(0, 48 + x, last_y, shape, HEIGHT);
    }
}

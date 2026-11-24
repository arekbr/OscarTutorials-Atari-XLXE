#include "../common/mux.h"

#define COUNT 16
static const byte image[] = {
    #embed spd_sprites "../../Resources/numballs.spd"
};
static const byte wave[] = { 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 1, 1, 2, 3 };
static byte shapes[COUNT][MUX_SIZE];

struct Sprite
{
    byte x, y, image;
};

static Sprite sprites[COUNT];

static void sort_by_y(void)
{
    for (byte i = 1; i < COUNT; i++)
    {
        Sprite item = sprites[i];
        byte j = i;
        while (j && sprites[j - 1].y > item.y)
        {
            sprites[j] = sprites[j - 1];
            j--;
        }
        sprites[j] = item;
    }
}

int main(void)
{
    for (byte n = 0; n < COUNT; n++)
    {
        byte full[21];
        player_shape(image + 64 * n, full);
        for (byte y = 0; y < MUX_SIZE; y++)
            shapes[n][y] = full[(word)y * 21 / MUX_SIZE];
    }

    mux_on(false);
    byte page = 96;
    byte phase = 0;
    for (;;)
    {
        mux_begin(page);
        // Reverse the initial order and give each object its own phase.
        // A free player can be reused when the next row's DLI is past
        // its old image. The 32-object variant allows only 24 lines per band.
        for (byte i = 0; i < COUNT; i++)
        {
            byte image = COUNT - 1 - i;
            byte band = image >> 2;
            byte slot = image & 3;
            byte offset = wave[(phase + 3 * slot + band) & 15];
            sprites[i].y = MUX_TOP + 48 * band + 2 + offset;
            sprites[i].x = 58 + 38 * slot + wave[(phase + 2 * band + slot) & 15];
            sprites[i].image = image;
        }
        sort_by_y();
        byte free_at[4] = { 0, 0, 0, 0 };
        for (byte i = 0; i < COUNT; i++)
        {
            byte y = sprites[i].y;
            byte dli_line = MUX_TOP + ((y - MUX_TOP) & 0xf8);
            for (byte player = 0; player < 4; player++)
            {
                if (free_at[player] > dli_line)
                    continue;
                mux_put(page, player, sprites[i].x, y, shapes[sprites[i].image]);
                free_at[player] = y + MUX_SIZE;
                break;
            }
        }
        mux_commit(page);

        page = 96 - page;
        phase++;
    }
}

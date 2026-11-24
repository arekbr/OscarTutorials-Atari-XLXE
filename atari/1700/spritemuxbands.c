#include "../common/mux.h"

#define COUNT 16
static const byte image[] = {
    #embed spd_sprites "../../Resources/numballs.spd"
};
static const byte wave[] = { 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 1, 1, 2, 3 };
static byte shapes[COUNT][MUX_SIZE];

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
        // Fixed bands reuse each physical player four times.
        for (byte band = 0; band < 4; band++)
        {
            byte y = MUX_TOP + 48 * band + 4 + (wave[(phase + 4 * band) & 15] >> 1);
            for (byte player = 0; player < 4; player++)
                mux_put(page, player, 62 + 36 * player, y, shapes[4 * band + player]);
        }
        mux_commit(page);

        page = 96 - page;
        phase++;
    }
}

#include "../common/shape.h"

static const byte SpriteImage[] = {
    #embed spd_sprites "../../Resources/beachball.spd"
};

int main(void)
{
    byte shape[4][21];
    for (byte frame = 0; frame < 4; frame++)
        player_shape(SpriteImage + 64 * frame, shape[frame]);
    sprite_stage();
    for (byte i = 0; i < 4; i++)
        PCOLOR(i) = player_colors[i];

    byte last[4] = { 0, 0, 0, 0 };
    byte offset = 0;
    byte counter = 0;
    for (;;)
    {
        wait_blank();
        for (byte i = 0; i < 4; i++)
        {
            byte y = 32 + (offset + 43 * i) % 170;
            byte frame = ((counter >> 2) + i) & 3;
            player_erase(i, last[i], 21);
            player_draw(i, 56 + 37 * i, y, shape[frame], 21);
            last[i] = y;
        }
        if (++offset == 170)
            offset = 0;
        counter++;
    }
}

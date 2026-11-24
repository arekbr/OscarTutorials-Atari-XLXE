#include "../common/shape.h"
#include <string.h>
#include <stdlib.h>

const char SpriteImage[64] = {
	0b00000000, 0b11111000, 0b00000000,
	0b00000011, 0b11111110, 0b00000000,
	0b00001111, 0b11111111, 0b10000000,
	0b00011111, 0b11111111, 0b11000000,
	0b00111111, 0b11111111, 0b11100000,
	0b00111111, 0b11111111, 0b11100000,
	0b01111111, 0b11111111, 0b11110000,
	0b01111111, 0b11111111, 0b11110000,
	0b11111111, 0b11111111, 0b11111000,
	0b11111111, 0b11111111, 0b11111000,
	0b11111111, 0b11111111, 0b11111000,
	0b11111111, 0b11111111, 0b11111000,
	0b11111111, 0b11111111, 0b11111000,
	0b01111111, 0b11111111, 0b11110000,
	0b01111111, 0b11111111, 0b11110000,
	0b00111111, 0b11111111, 0b11100000,
	0b00111111, 0b11111111, 0b11100000,
	0b00011111, 0b11111111, 0b11000000,
	0b00001111, 0b11111111, 0b10000000,
	0b00000011, 0b11111110, 0b00000000,
	0b00000000, 0b11111000, 0b00000000
};

// Screen memory
char * const Screen = (char *)0x0400;

static byte shape[21];
static byte last_y[4];

static void move_sprite(byte slot, byte x, byte y)
{
    player_erase(slot, last_y[slot], 21);
    player_draw(slot, 48 + x, 24 + y, shape, 21);
    last_y[slot] = 24 + y;
}

struct RefSprite
{
	int sx, sy, vx, vy;
}	sprites[4];

#define FBITS	4
#define	FMUL	(1 << FBITS)

int main(void)
{
	// Convert the artwork to a GTIA player.
	player_shape((const byte *)SpriteImage, shape);

	// Initialize sprite data structures
	sprite_stage();

	for(char i=0; i<4; i++)
	{
		// Random position
		sprites[i].sx = (rand() % (160 - 8)) << FBITS;
		sprites[i].sy = (rand() % (192 - 21)) << FBITS;

		// Random speed
		sprites[i].vx = rand() % 81 - 40;
		sprites[i].vy = rand() % 81 - 40;

		// Initialize sprite
		PCOLOR(i) = player_colors[i];
		move_sprite(i, (sprites[i].sx >> FBITS), (sprites[i].sy >> FBITS));
	}

	while (true)
	{
		// Snapshot GTIA player collisions, then clear the latches.
		byte collisions[4];
        for (byte i = 0; i < 4; i++)
            collisions[i] = REG8(0xd00c + i);
        HITCLR = 0;

		for(char i=0; i<4; i++)
		{
			// Advance position
			int sx = sprites[i].sx + sprites[i].vx;
			int sy = sprites[i].sy + sprites[i].vy;

			// Check for reflection at left/right border
			if (sx < 0 || sx > ((160 - 8) << FBITS))
				sprites[i].vx = -sprites[i].vx;
			else
				sprites[i].sx = sx;

			// Check for reflection at top/bottom border
			if (sy < 0 || sy > ((192 - 21) << FBITS))
				sprites[i].vy = -sprites[i].vy;
			else
				sprites[i].sy = sy;

			// Update sprite position
			move_sprite(i, (sprites[i].sx >> FBITS), (sprites[i].sy >> FBITS));

			// Colorize colliding sprites yellow
			PCOLOR(i) = collisions[i] ? 0x1e : player_colors[i];

			// Apply gravity to vertical velocity
			if (sprites[i].vy < 96) sprites[i].vy++;
		}

		// Wait one frame
		wait_blank();
	}

	return 0;
}

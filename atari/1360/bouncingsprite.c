#include "../common/shape.h"
#include <string.h>
#include <stdlib.h>
#include <fixmath.h>

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

#define FBITS	3
#define FRADIUS	(10 << FBITS)
#define FROUND	(1 << (FBITS - 1))

#define FBITS2	4
#define FROUND2	(1 << (FBITS2 - 1))
#define FBITS3	5
#define FROUND3	(1 << (FBITS3 - 1))

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
		sprites[i].vx = rand() % 101 - 50;
		sprites[i].vy = rand() % 101 - 50;

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
			if (sy < 0)
				sprites[i].vy = -sprites[i].vy;
			else if (sy > ((192 - 21) << FBITS))
				sprites[i].vy = 1 - sprites[i].vy;
			else
				sprites[i].sy = sy;

			// Update sprite position
			move_sprite(i, (sprites[i].sx >> FBITS), (sprites[i].sy >> FBITS));

			// Check if sprite in collision set
			if (collisions[i])
			{
				// Check with all further sprites in collision set
				for(char j=i+1; j<4; j++)
				{
					// Both sprites are in collision set
					if (collisions[i] & (1 << j))
					{
						// Distance vector from center to center
						int dx = sprites[i].sx - sprites[j].sx;
						int dy = sprites[i].sy - sprites[j].sy;

						// Check bounding box collision
						if (dx >= -2 * FRADIUS && dx <= 2 * FRADIUS && dy >= -2 * FRADIUS && dy < 2 * FRADIUS)
						{
							// distance square
							unsigned	dd = dx * dx + dy * dy;

							// Check distance center to center for collision
							if (dd && dd <= 4 * FRADIUS * FRADIUS)
							{
								// Divide distance vector by its length to get normalized vector
								int	dds = usqrt(dd);
								int nx = ((dx << FBITS2) + (dds >> 1)) / dds;
								int ny = ((dy << FBITS2) + (dds >> 1)) / dds;

								// Calculate velocity of both balls along the distance vector
								int vi = sprites[i].vx * nx + sprites[i].vy * ny;
								int vj = sprites[j].vx * nx + sprites[j].vy * ny;

								// Checking if actually colliding and not moving appart
								if (vi - vj < 0)
								{
									int vij = (vi - vj + FROUND) >> FBITS;

									// Apply rules of non elastic collision
									sprites[i].vx -= (nx * vij + FROUND3) >> FBITS3;
									sprites[i].vy -= (ny * vij + FROUND3) >> FBITS3;
									sprites[j].vx += (nx * vij + FROUND3) >> FBITS3;
									sprites[j].vy += (ny * vij + FROUND3) >> FBITS3;
								}
							}
						}
					}
				}
			}

			// Apply gravity to vertical velocity
			if (sprites[i].vy < 96) sprites[i].vy++;
		}

		// Wait one frame
		wait_blank();
	}

	return 0;
}

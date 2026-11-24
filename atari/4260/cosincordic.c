#include "../common/pmg.h"
#include <math.h>
#include <string.h>

static const byte crosshair[] = {
    0x10, 0x10, 0x10, 0x10, 0x00, 0xef, 0x00, 0x10, 0x10, 0x10, 0x10
};
static const byte CenterX = 124;
static const byte CenterY = 114;


// Cordic atan table
__striped static const int arortab[8] = {8192, 4836, 2555, 1297, 651, 326, 163, 81};

// Calculate sine and cosine using cordic
void cosincc(int w, signed char * si, signed char * co)
{
	// Scaled start vector
	int dx = 9945;
	int dy = 0;

	// Move into correct quadrant
	if (w > 16384 || w < -16384)
	{
		w ^= 0x8000;
		dx = -dx;
	}

	// Eight iterations, reduce to sacrifice precision for speed
	for(char i=0; i<8; i++)
	{
		// Rotation vector
		int sx = dx >> i;
		int sy = dy >> i;

		// Check foward or backward rotation
		if (w > 0)
		{
			// Forward rotation
			dx += sy;
			dy -= sx;
			w -= arortab[i];
		}
		else
		{
			// Backward rotation
			dx -= sy;
			dy += sx;
			w += arortab[i];
		}
	}

	// Return MSBs of coordinates as cos and sin
	*si = dy >> 8;
	*co = dx >> 8;
}

int main(void)
{
    pmg_on();
    PCOLOR(0) = 0x0e;
    PCOLOR(1) = 0x1a;
    player_draw(1, CenterX, CenterY, crosshair, sizeof(crosshair));
    byte old_y = CenterY;

	// Eight bit angle
	char w = 0;
	for(;;)
	{
		signed char sx, sy;

		// Calculate position on circle using cordic
		COLBK = 0x42;
		cosincc(w << 8, &sx, &sy);
		COLBK = COLOR(4);

		// Move sprite
		player_erase(0, old_y, sizeof(crosshair));
        old_y = CenterY + sy;
        player_draw(0, CenterX + sx, old_y, crosshair, sizeof(crosshair));

		// Advance
		w++;

		wait_blank();
	}

	return 0;

}

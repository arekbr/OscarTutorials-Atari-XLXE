#include "../common/pmg.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static const byte crosshair[] = {
    0x10, 0x10, 0x10, 0x10, 0x00, 0xef, 0x00, 0x10, 0x10, 0x10, 0x10
};
static const byte CenterX = 124;
static const byte CenterY = 114;


// Table of inverse tangens
static char atab[16][16];

// Lookup inverse tangens in first quadrant
unsigned uatan(unsigned dx, unsigned dy)
{
	// Shift to right until it fits into
	// lookup table, first shift uses byte optimization
	if ((dx | dy) & 0xf000)
	{
		dx >>= 8;
		dy >>= 8;
	}
	// Shift out all remaining bits from high byte in one step
	if ((dx | dy) & 0xff00)
	{
		dx >>= 4;
		dy >>= 4;
	}

	// Switch to bytes for the rest
	char ux = dx, uy = dy;
	while ((ux | uy) & 0xf0)
	{
		ux >>= 1;
		uy >>= 1;
	}

	// Lookup inverse tangens for the remaining values
	return atab[uy][ux];
}

// Lookup inverse tangens for all quadrants
int matan(int dx, int dy)
{
	if (dx < 0)
	{
		if (dy < 0)
			return 0x80 + uatan(-dx, -dy);
		else
			return 0x80 - uatan(-dx, dy);
	}
	else if (dy < 0)
		return 0x100 - uatan(dx, -dy);
	else
		return uatan(dx, dy);
}

int main(void)
{
	// Init lookup table
	for(char y=0; y<16; y++)
		for(char x=0; x<16; x++)
			atab[y][x] = (char)(atan2(y, x) * 128 / PI + 0.5);

    pmg_on();
    PCOLOR(0) = 0x0e;
    PCOLOR(1) = 0x1a;
    player_draw(1, CenterX, CenterY, crosshair, sizeof(crosshair));
    byte old_y = CenterY;

	// Relative sprite position
	int sx = 0, sy = 0;
	for(;;)
	{
		// Move sprite with joystick
		int nx = sx + joy_x();
        int ny = sy + joy_y();
        if (nx >= -70 && nx <= 70) sx = nx;
        if (ny >= -75 && ny <= 75) sy = ny;
		player_erase(0, old_y, sizeof(crosshair));
        old_y = CenterY + sy;
        player_draw(0, CenterX + sx, old_y, crosshair, sizeof(crosshair));

		// Calculate inverse tangens of sprite position, use border color
		// to show raster time used
		COLBK = 0x42;
		int ai = matan(sx, sy);
		COLBK = COLOR(4);

		// Reference value
		int af = (int)(atan2(sy, sx) * 128 / PI) & 0xff;

		// Print result
		printf("%4d, %4d, %3d : %3d\n", sx, sy, ai, af);

		wait_blank();
	}

	return 0;
}

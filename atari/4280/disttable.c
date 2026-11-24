#include "../common/pmg.h"
#include <stdio.h>
#include <string.h>
#include <fixmath.h>

static const byte crosshair[] = {
    0x10, 0x10, 0x10, 0x10, 0x00, 0xef, 0x00, 0x10, 0x10, 0x10, 0x10
};
static const byte CenterX = 124;
static const byte CenterY = 114;


static char dtab[16][16];

int mdist(int dx, int dy)
{
	// Remove sign
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;

	unsigned ux = dx, uy = dy;
	char	s = 0;

	// Shift to right until it fits into
	// lookup table, first shift uses byte optimization
	if ((ux | uy) & 0xf000)
	{
		ux >>= 8;
		uy >>= 8;
		s += 8;
	}
	// Shift out all remaining bits from high byte in one step
	if ((ux | uy) & 0xff00)
	{
		ux >>= 4;
		uy >>= 4;
		s += 4;
	}

	// Switch to bytes for the rest
	char bx = ux, by = uy;
	while ((bx | by) & 0xf0)
	{
		bx >>= 1;
		by >>= 1;
		s++;
	}

	// Lookup distance function in table, and shift back into
	// original range
	return dtab[by][bx] << s;
}

int main(void)
{
	// Init lookup table
	for(char y=0; y<16; y++)
		for(char x=0; x<16; x++)
			dtab[y][x] = usqrt(x * x + y * y);

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

		// Calculate distance from center using lookup table
		COLBK = 0x42;
		int d = mdist(sx, sy);
		COLBK = COLOR(4);

		// Print result
		printf("%4d, %4d, %4d\n", sx, sy, d);

		wait_blank();
	}

	return 0;
}

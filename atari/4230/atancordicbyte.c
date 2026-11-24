#include "../common/pmg.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static const byte crosshair[] = {
    0x10, 0x10, 0x10, 0x10, 0x00, 0xef, 0x00, 0x10, 0x10, 0x10, 0x10
};
static const byte CenterX = 124;
static const byte CenterY = 114;


// Cordic atan table
static const char arortab[6] = {32, 19, 10, 5, 3, 1};

// Calculate inverse tangens using cordic with 8 bit precision
char matanu(char dx, signed char dy)
{
	// Accumulated angle
	signed char	sum = 0;

	// Sacrifice one bit of precision to avoid overflow
	if (dx & 0x80)
	{
		dx >>= 1;
		dy >>= 1;
	}

	// First step of cordic
	char sx = dx;
	signed char sy = dy;

	if (dy > 0)
	{
		dx += sy;
		dy -= sx;
		sum = arortab[0];
	}
	else if (dy < 0)
	{
		dx -= sy;
		dy += sx;
		sum = -arortab[0];
	}

	// Remaining iterations, reduce to trade off precision for speed
	for(char i=1; i<6; i++)
	{
		char sx = dx >> i;
		signed char sy = dy >> i;

		if (dy > 0)
		{
			dx += sy;
			dy -= sx;
			sum += arortab[i];
		}
		else if (dy < 0)
		{
			dx -= sy;
			dy += sx;
			sum -= arortab[i];
		}
	}

	return sum;
}

// Inverse tangens four all four quadrants
char matan(int dx, signed char dy)
{
	if (dx < 0)
		return 128 - matanu(-dx, dy);
	else
		return matanu( dx, dy);
}

int main(void)
{
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

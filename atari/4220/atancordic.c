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
__striped static int arortab[8] = {8192, 4836, 2555, 1297, 651, 325, 162, 81};

// Calculate inverse tangens using cordic
int matan(int dx, int dy)
{
	// Accumulated angle
	int sum = 0;

	// Fix second and third quadrant
	if (dx < 0)
	{
		dx = -dx;
		dy = -dy;
		sum = -32768;
	}

	// Loop for seven bits precision, use shorter loop to
	// trade precision for speed
	for(char i=0; i<7; i++)
	{
		// Rotation vector
		int sx = dx >> i;
		int sy = dy >> i;

		// Check for forward/backward rotation
		if (dy > 0)
		{
			// Forward rotation
			dx += sy;
			dy -= sx;
			sum += arortab[i];
		}
		else if (dy < 0)
		{
			// Backward rotation
			dx -= sy;
			dy += sx;
			sum -= arortab[i];
		}
	}

	// Get rid of LSB
	return (sum >> 8) & 0xff;
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

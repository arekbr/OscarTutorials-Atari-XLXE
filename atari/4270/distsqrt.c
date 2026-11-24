#include "../common/pmg.h"
#include <stdio.h>
#include <string.h>
#include <fixmath.h>

static const byte crosshair[] = {
    0x10, 0x10, 0x10, 0x10, 0x00, 0xef, 0x00, 0x10, 0x10, 0x10, 0x10
};
static const byte CenterX = 124;
static const byte CenterY = 114;


int main(void)
{
    pmg_on();
    PCOLOR(0) = 0x0e;
    PCOLOR(1) = 0x1a;
    player_draw(1, CenterX, CenterY, crosshair, sizeof(crosshair));
    byte old_y = CenterY;

	// Sprite coordinates
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

		// Calculate distance from center using integer square root
		COLBK = 0x42;
		int d = usqrt(sx * sx + sy * sy);
		COLBK = COLOR(4);

		// Print result
		printf("%4d, %4d, %4d\n", sx, sy, d);

		wait_blank();
	}

	return 0;
}

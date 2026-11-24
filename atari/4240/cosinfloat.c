#include "../common/pmg.h"
#include <math.h>
#include <string.h>

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

	// Eight bit angle
	char w = 0;
	for(;;)
	{
		// Calculate position on circle using sin and cos float operations
		COLBK = 0x42;
		int sx = (int)(64 * cos(w * PI / 128.0));
		int sy = (int)(64 * sin(w * PI / 128.0));
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

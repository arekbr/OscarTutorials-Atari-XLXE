#include <math.h>
#include "../common/bitmap.h"

int main(void)
{
    clear_dma(BITMAP0, 8192);
    bitmap_display(15, BITMAP0);
	float	w = 0.0;
	for(;;)
	{
		// Some fancy curves
		int x = (int)(160 + cos(w) * 80 + cos(w * 5) * 20 + cos(w * 13) * 10);
		int y = (int)(96 + sin(w * 2) * 68 + sin(w * 7) * 16 + sin(w * 11) * 8);

		// Set the pixel
		pixel(BITMAP0, x, y, SET_PIXELS);

		// Advance
		w += 0.01;
	}

	return 0;
}

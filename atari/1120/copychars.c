#include <stdlib.h>
#include "../common/display.h"

const char Smiley[] = {
	0b01111100,
	0b11111110,
	0b10010010,
	0b11111110,
	0b10000010,
	0b11000110,
	0b01111100,
	0b00000000,

	0b01111100,
	0b11111110,
	0b10010010,
	0b11111110,
	0b11000110,
	0b10111010,
	0b01111100,
	0b00000000,

};

int main(void)
{
    copy_dma(FONT, ROM_FONT, 1024);
    copy_dma(FONT + 8 * screen_code('A'), (const byte *)Smiley, sizeof(Smiley));
    for (word i = 0; i < 960; i++)
        TEXT[i] = screen_code('A') + (rand() & 1);
    text_display(2, true);
    COLOR(1) = 0x1e;
    for (;;) {}
}

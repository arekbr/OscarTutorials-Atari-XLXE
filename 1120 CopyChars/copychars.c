#include <string.h>
#include <stdlib.h>
#include <c64/vic.h>
#include <c64/memmap.h>

// Custom screen address
char * const Screen = (char *)0xc000;

// Custom charset address
char * const Charset = (char *)0xd000;

// Color mem address
char * const Color = (char *)0xd800;

// Two custom characters
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
	// Install memory trampoline to ensure system
	// keeps running when playing with the PLA
	mmap_trampoline();

	// Swap in char ROM
	mmap_set(MMAP_CHAR_ROM);
	// Copy system charset into RAM underneath char ROM
	memcpy(Charset, Charset, 2048);	
	// Copy custom chars
	memcpy(Charset + 128 * 8, Smiley, sizeof(Smiley));
	// Swap out char ROM
	mmap_set(MMAP_ROM);

	// Fill screen with all chars
	for(unsigned i=0; i<1000; i++)	
		Screen[i] = i;
	
	// Color all cells yellow
	memset(Color, VCOL_YELLOW, 1000);

	// Background and border black
	vic.color_border = VCOL_BLACK;
	vic.color_back = VCOL_BLACK;

	// Set VIC to show custom screen with custom charset
	vic_setmode(VICM_TEXT, Screen, Charset);

	// Loop forever
	while (true)
		;

	return 0;
}

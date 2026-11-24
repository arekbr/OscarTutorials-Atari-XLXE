#include "../common/console.h"

// Expand a character cell row data into text
void expand8(char c, char * s)
{
	// Loop over all eight pixel
	for(char i=0; i<8; i++)
	{
		// Check MSB and select full/empty char
		s[i] = (c & 0x80) ? '#' : '.';
		// Next bit
		c <<= 1;
	}
	// Close string
	s[8] = 0;
}

int main(void)
{
	const char *romp = (const char *)0xe000;
	for (word i = 0; i < 1024; i++)
	{
		// String for one row of char data
		char	exs[9];

		char c = romp[i];

		// Expand the char row into string
		expand8(c, exs);

		// Print line
		printf("%03x : %02x - %s\n", i, c, exs);
	}

	return 0;
}

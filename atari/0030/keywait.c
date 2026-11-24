#include <stdio.h>
#include "../common/hw.h"

int main(void)
{
	// Long counter
	long	li = 0;

	// Loop until a key is pressed
	KEYCODE = 255;
	while (KEYCODE == 255)
	{
		printf("PRESS KEY TO STOP %ld\n", li);
		li++;
	}
	return 0;
}

#include <stdlib.h>
#include <string.h>
#include "../common/display.h"

#define maze_grid TEXT

void maze_border(char fill, char border)
{
	// Fill inner area
	for (word i = 0; i < 24 * 40; i++)
		maze_grid[i] = fill;

	// Fill borders
	for(int i=0; i<24; i++)
	{
		maze_grid[i * 40 +  0] = border;
		maze_grid[i * 40 + 39] = border;
	}

	for(int i=0; i<40; i++)
	{
		maze_grid[i          ] = border;
		maze_grid[i + 40 * 23] = border;
	}
}

const int bdir[4] = {1, 40, -1, -40};

bool maze_check(int p, char d)
{
	int p1 = p + bdir[d];
	int p2 = p1 + bdir[d];

	int	d0 = bdir[(d + 1) & 3], d1 = bdir[(d + 3) & 3];

	return
		maze_grid[p1] == 0xff &&
		maze_grid[p1 + d0] >= 0xfe &&
		maze_grid[p1 + d1] >= 0xfe &&
		maze_grid[p2] >= 0xfe &&
		maze_grid[p2 + d0] >= 0xfe &&
		maze_grid[p2 + d1] >= 0xfe;
}

void maze_build(void)
{
	maze_border(0xff, 0xfe);

	// Start position of path
	int	p = 40 * 12 + 20;

	// Mark start field
	maze_grid[p] = 0xfc;

	for(;;)
	{
		// Random direction to look at first
		char d = rand() & 3;

		char i = 0;

		// Rotation to look around if no path forward
		char dd = 1 + (rand() & 2);

		// Find a path forward by looking in all four directions
		while (i < 4 && !maze_check(p, d))
		{
			d = (d + dd) & 3;
			i++;
		}

		if (i == 4)
		{
			// No path possible, backtrack
			if (maze_grid[p] != 0xfc)
				p -= bdir[maze_grid[p]];

			if (maze_grid[p] == 0xfc)
			{
				// Back at start location, clear it
				maze_grid[p] = 0;

				// Fill maze with alternating color blocks
				for(int i=0; i<24; i++)
				{
					for(int j=0; j<40; j++)
					{
						if (maze_grid[i * 40 + j] > 0x80)
							maze_grid[i * 40 + j] = 0x80;
						else
							maze_grid[i * 40 + j] = 0;
					}
				}

				return;
			}
		}
		else
		{
			// Move along selected direction
			p += bdir[d];
			// Remember direction, that brought us here for back tracking
			maze_grid[p] = d;
		}
	}
}


int main(void)
{

	text_display(2, false);

	while (true)
	{
		maze_build();
		any_key();
	}

	return 0;
}

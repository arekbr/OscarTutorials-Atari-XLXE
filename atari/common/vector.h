#ifndef TUTORIAL_VECTOR_H
#define TUTORIAL_VECTOR_H

#include <fixmath.h>
#include <math.h>
#include "bitmap.h"

struct Point { int x, y; };

__striped static const int SinTab[256] = {
	#for(i, 256) int(sin(i * PI / 128) * 256),
};

static void star_points(Point *corners, byte angle, byte size)
{
    byte radius = 14 + (size >> 2);
    for (byte i = 0; i < 10; i++)
    {
        int x = lmul8f8s(SinTab[angle], radius);
        int y = lmul8f8s(SinTab[(angle + 64) & 255], radius);
        if (i & 1)
        {
            x >>= 1;
            y >>= 1;
        }
        corners[i].x = 160 + x;
        corners[i].y = 96 + y;
        angle += 26;
    }
}

static __noinline void line(volatile byte *base, const Point *a, const Point *b, DrawMode mode)
{
    int x = a->x, y = a->y;
    int end_x = b->x, end_y = b->y;
    int dx = abs(end_x - x), dy = -abs(end_y - y);
    int sx = x < end_x ? 1 : -1;
    int sy = y < end_y ? 1 : -1;
    int error = dx + dy;
    byte bit = 0x80 >> (x & 7);
    volatile byte *p = bitmap_row(base, y) + (x >> 3);

    // Exclude the endpoint so each vertex belongs to only one edge.
    // Move the byte pointer directly, skipping the ANTIC 4 KB gap.
    while (x != end_x || y != end_y)
    {
        if (mode == SET_PIXELS)
            *p |= bit;
        else if (mode == XOR_PIXELS)
            *p ^= bit;
        else
            *p &= ~bit;

        int twice = 2 * error;
        if (twice >= dy)
        {
            error += dy;
            x += sx;
            if (sx > 0)
            {
                bit >>= 1;
                if (!bit)
                {
                    bit = 0x80;
                    p++;
                }
            }
            else
            {
                bit <<= 1;
                if (!bit)
                {
                    bit = 1;
                    p--;
                }
            }
        }
        if (twice <= dx)
        {
            error += dx;
            if (sy > 0)
            {
                p += y == 95 ? 296 : 40;
                y++;
            }
            else
            {
                p -= y == 96 ? 296 : 40;
                y--;
            }
        }
    }
}

static void star_draw(volatile byte *base, const Point *corners, DrawMode mode)
{
    byte previous = 9;
    for (byte i = 0; i < 10; i++)
    {
        line(base, corners + previous, corners + i, mode);
        previous = i;
    }
}

#endif

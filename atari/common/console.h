#ifndef TUTORIAL_CONSOLE_H
#define TUTORIAL_CONSOLE_H

#include <stdio.h>
#include "hw.h"

// Oscar64 returns the E: line terminator as ATASCII $9b.
static char input_char(void)
{
    char c = getchar();
    return c == 0x9b ? '\n' : c;
}

static void read_line(char *buf, byte capacity)
{
    byte size = 0;
    char c;
    while ((c = input_char()) != '\n')
    {
        if (size + 1 < capacity)
            buf[size++] = c;
    }
    buf[size] = 0;
}

static float ticks_per_second(void)
{
    return (REG8(0xd014) & 14) ? 60.0 : 50.0;
}

#endif

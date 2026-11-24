#ifndef TUTORIAL_NAVIGATION_H
#define TUTORIAL_NAVIGATION_H

#include "hw.h"

#define NAV_UP 1
#define NAV_DOWN 2
#define NAV_LEFT 4
#define NAV_RIGHT 8

static byte navigation(void)
{
    byte key = KEYCODE;
    KEYCODE = 255;
    // Atari cursor keys include the Ctrl bit. Ignore Shift for both
    // the cursor keys and W/A/S/D.
    switch (key & 0xbf)
    {
        case 0x8e: case 0x2e: return NAV_UP;
        case 0x8f: case 0x3e: return NAV_DOWN;
        case 0x86: case 0x3f: return NAV_LEFT;
        case 0x87: case 0x3a: return NAV_RIGHT;
    }
    return ~STICK & 15;
}

static __forceinline signed char navigation_x(byte direction)
{
    return (direction & NAV_LEFT) ? -1 : ((direction & NAV_RIGHT) != 0);
}

static __forceinline signed char navigation_y(byte direction)
{
    return (direction & NAV_UP) ? -1 : ((direction & NAV_DOWN) != 0);
}

#endif

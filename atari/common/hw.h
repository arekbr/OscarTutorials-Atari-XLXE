#ifndef TUTORIAL_HW_H
#define TUTORIAL_HW_H

#include <string.h>

typedef unsigned char byte;
typedef unsigned int word;

#define REG8(address) (*(volatile byte *)(address))
#define REG16(address) (*(volatile word *)(address))

#define RTCLOK REG8(0x14)
#define KEYCODE REG8(0x02fc)
#define STICK REG8(0x0278)
#define TRIGGER REG8(0x0284)
#define VCOUNT REG8(0xd40b)
#define WSYNC REG8(0xd40a)
#define NMIEN REG8(0xd40e)
#define HSCROL REG8(0xd404)
#define VSCROL REG8(0xd405)
#define DMACTL REG8(0xd400)
#define SDMCTL REG8(0x022f)
#define COLBK REG8(0xd01a)
#define COLPF(n) REG8(0xd016 + (n))
#define COLOR(n) REG8(0x02c4 + (n))
#define CHBASE REG8(0xd409)
#define CHBAS REG8(0x02f4)

static void wait_frame(void)
{
    byte frame = RTCLOK;
    while (RTCLOK == frame) {}
}

static void wait_blank(void)
{
    while (VCOUNT >= 120) {}
    while (VCOUNT < 120) {}
}

static signed char joy_x(void)
{
    return ((STICK & 4) == 0) ? -1 : ((STICK & 8) == 0);
}

static signed char joy_y(void)
{
    return ((STICK & 1) == 0) ? -1 : ((STICK & 2) == 0);
}

static void any_key(void)
{
    KEYCODE = 255;
    while (KEYCODE == 255) {}
    KEYCODE = 255;
}

static __forceinline byte screen_code(byte c)
{
    // ATASCII ranges 00-1f, 20-5f and 60-7f have distinct offsets.
    byte base = c & 127;
    if (base < 32)
        base += 64;
    else if (base < 96)
        base -= 32;
    return base | (c & 128);
}

static void write_text(volatile byte *screen, const char *text)
{
    while (*text)
        *screen++ = screen_code(*text++);
}

#endif

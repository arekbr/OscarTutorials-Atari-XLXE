#include "../common/hw.h"

static byte peek(word address)
{
    return REG8(address);
}

static void poke(word address, byte value)
{
    REG8(address) = value;
}

int main(void)
{
    for (;;)
    {
        poke(0xd01a, 0x02);
        while (peek(0xd40b) < 68) {}
        poke(0xd01a, 0x0e);
        while (peek(0xd40b) >= 68) {}
    }
}

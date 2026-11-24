#include "../common/hw.h"

int main(void)
{
    volatile byte *screen = (volatile byte *)REG16(0x58);
    for (word i = 0; i < 256; i++)
        screen[80 + i] = i;
    return 0;
}

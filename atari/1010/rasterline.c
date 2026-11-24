#include "../common/hw.h"

int main(void)
{
    for (;;)
        COLBK = VCOUNT & 0xf0 | 8;
}

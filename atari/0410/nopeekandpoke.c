#include "../common/hw.h"

int main(void)
{
    for (;;)
    {
        COLBK = 0x02;
        while (VCOUNT < 68) {}
        COLBK = 0x0e;
        while (VCOUNT >= 68) {}
    }
}

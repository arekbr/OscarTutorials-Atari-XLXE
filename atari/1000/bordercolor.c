#include "../common/hw.h"

int main(void)
{
    for (;;)
    {
        wait_frame();
        COLOR(4) += 2;
    }
}

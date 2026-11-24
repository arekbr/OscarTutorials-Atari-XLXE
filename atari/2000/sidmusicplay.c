#include <stdio.h>
#include "../common/music.h"

int main(void)
{
    puts("POKEY MUSIC - FRAME POLLING\n");
    music_start();
    for (;;)
    {
        wait_frame();
        __asm { jsr music_tick }
    }
}

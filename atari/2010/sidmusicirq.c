#include <stdio.h>
#include "../common/music.h"

int main(void)
{
    music_start();
    install_vbi(7, music_vbi);
    for (;;)
    {
        puts("POKEY PLAYS DURING CONSOLE OUTPUT\n");
        for (byte frame = 0; frame < 20; frame++)
            wait_frame();
    }
}

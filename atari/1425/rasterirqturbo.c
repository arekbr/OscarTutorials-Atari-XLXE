#include <stdio.h>
#include <time.h>
#include "../common/console.h"

// Atari XL/XE has a fixed CPU clock. ANTIC DMA steals CPU cycles.
// Run the same RAM loop with the normal playfield and with DMA off.
static volatile byte scratch[512];

static clock_t measure(void)
{
    clock_t start = clock();
    for (byte n = 0; n < 100; n++)
        for (word x = 0; x < sizeof(scratch); x++)
            scratch[x] = n ^ x;
    return clock() - start;
}

int main(void)
{
    byte original = SDMCTL;
    clock_t enabled = measure();
    SDMCTL = 0;
    wait_frame();
    clock_t disabled = measure();
    SDMCTL = original;
    wait_frame();
    printf("ANTIC DMA timing\n");
    printf("Display on:  %ld frames\n", enabled);
    printf("Display off: %ld frames\n", disabled);
    return 0;
}

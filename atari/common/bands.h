#ifndef TUTORIAL_BANDS_H
#define TUTORIAL_BANDS_H

#include "scroll.h"
#include "interrupt.h"

static volatile byte band_step;
static volatile byte band_count;
static volatile byte band_fine[4];
static volatile byte band_hue[4];

__asm bands_dli
{
    pha
    txa
    pha
    ldx band_step
    lda band_fine,x
    sta $d40a
    sta $d404
    lda band_hue,x
    sta $d018
    inx
    cpx band_count
    bne save
    ldx #0
save:
    stx band_step
    pla
    tax
    pla
    rti
}

static void bands_on(byte count)
{
    display_on(0x22);
    wait_blank();
    band_count = count;
    band_step = 0;
    install_dli(bands_dli);
}

#endif

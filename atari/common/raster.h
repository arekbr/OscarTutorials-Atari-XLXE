#ifndef TUTORIAL_RASTER_H
#define TUTORIAL_RASTER_H

#include "display.h"
#include "interrupt.h"

static volatile byte raster_step;
static volatile byte raster_count;
static volatile byte raster_border[24];
static volatile byte raster_background[24];

__asm color_dli
{
    pha
    txa
    pha
    ldx raster_step
    lda raster_border,x
    sta $d40a
    sta $d01a
    lda raster_background,x
    sta $d018
    inx
    cpx raster_count
    bne save
    ldx #0
save:
    stx raster_step
    pla
    tax
    pla
    rti
}

static void start_color_dli(byte count)
{
    wait_blank();
    raster_step = 0;
    raster_count = count;
    install_dli(color_dli);
}

#endif

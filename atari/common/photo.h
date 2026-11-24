#ifndef TUTORIAL_PHOTO_H
#define TUTORIAL_PHOTO_H

#include <oscar.h>
#include "display.h"
#include "interrupt.h"

static const char photo[] = {
    #embed lzo "../../atari/assets/blumba.antic"
};

static const byte photo_palette[] = {
    #embed "../../atari/assets/blumba.colors"
};

static volatile byte photo_band;
static volatile byte photo_limit;

__asm photo_dli
{
    pha
    txa
    pha
    tya
    pha
    ldx photo_band
    lda photo_palette + 1,x
    ldy photo_palette + 2,x
    pha
    lda photo_palette + 3,x
    pha
    txa
    clc
    adc #4
    cmp photo_limit
    bcc store
    lda #0
store:
    sta photo_band
    pla
    tax
    pla
    sta $d40a
    sta $d016
    sty $d017
    stx $d018
    pla
    tay
    pla
    tax
    pla
    rti
}

static void photo_list(byte rows, bool split)
{
    list_begin();
    DL[2] |= 0x80;
    list_line(14, BITMAP0);
    for (byte y = 1; y < rows; y++)
    {
        byte cmd = 14;
        if ((y & 7) == 7 && y + 1 < rows)
            cmd |= 0x80;
        if (y == 96)
            list_line(cmd, BITMAP0 + 0x1000);
        else
            DL[list_pos++] = cmd;
    }
    if (split)
    {
        DL[list_pos++] = 0x70;
        list_line(0x12, TEXT);
    }
    list_end();
}

static void photo_start(byte rows, bool split)
{
    display_off();
    oscar_expand_lzo((char *)BITMAP0, photo);
    photo_list(rows, split);
    CHBAS = 0xe0;
    COLOR(4) = photo_palette[0];
    COLOR(1) = 0x0e;
    COLOR(2) = 0x82;
    display_on(0x22);
    wait_blank();
    photo_band = 0;
    photo_limit = rows >> 1;
    install_dli(photo_dli);
}

#endif

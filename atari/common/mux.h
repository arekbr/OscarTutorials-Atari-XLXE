#ifndef TUTORIAL_MUX_H
#define TUTORIAL_MUX_H

#include "shape.h"
#include "interrupt.h"

#define MUX_TOP 32
#define MUX_ROWS 24
#define MUX_SIZE 12
#define MUX_ALT ((volatile byte *)0x9800)

static volatile byte mux_row;
static volatile byte front_page;
static volatile byte new_page;
static volatile byte frame_ready;
static volatile byte xpos[192]; // 2 banks of 24 rows x 4 players

__asm mux_dli
{
    pha
    txa
    pha
    lda mux_row
    clc
    adc front_page
    tax
    lda xpos,x
    sta $d40a
    sta $d000
    lda xpos + 1,x
    sta $d001
    lda xpos + 2,x
    sta $d002
    lda xpos + 3,x
    sta $d003
    lda mux_row
    clc
    adc #4
    cmp #96
    bcc save
    lda #0
save:
    sta mux_row
    pla
    tax
    pla
    rti
}

__asm mux_vbi
{
    lda frame_ready
    beq repeat
    lda new_page
    sta front_page
    beq first
    lda #$98
    bne set_page
first:
    lda #$90
set_page:
    sta $d407
    lda #0
    sta frame_ready
repeat:
    jmp $e462
}

static void mux_on(bool vbi)
{
    sprite_stage();
    wait_blank();
    DL[2] |= 0x80;
    DL[3] |= 0x80;
    for (byte row = 1; row < 23; row++)
        DL[5 + row] |= 0x80;
    for (byte i = 0; i < 4; i++)
        PCOLOR(i) = player_colors[i];
    install_dli(mux_dli);
    if (vbi)
        install_vbi(7, mux_vbi);
}

static void mux_begin(byte page)
{
    volatile byte *pm = page ? MUX_ALT : PMRAM;
    clear_dma(pm + 0x400, 0x400);
    for (byte i = 0; i < 96; i++)
        xpos[page + i] = 0;
}

static void mux_put(byte page, byte slot, byte x, byte y, const byte *shape)
{
    volatile byte *pm = page ? MUX_ALT : PMRAM;
    volatile byte *target = pm + 0x400 + (word)slot * 256 + y;
    copy_dma(target, shape, MUX_SIZE);
    byte row = (y - MUX_TOP) >> 3;
    for (byte i = row; i < MUX_ROWS; i++)
        xpos[page + 4 * i + slot] = x;
}

static void mux_commit(byte page)
{
    wait_blank();
    front_page = page;
    REG8(0xd407) = page ? 0x98 : 0x90;
}

#endif

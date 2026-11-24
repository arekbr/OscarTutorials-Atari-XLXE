#ifndef TUTORIAL_MUSIC_H
#define TUTORIAL_MUSIC_H

#include "interrupt.h"

#ifdef MUSIC_REGION
#pragma data(music)
#endif

static const byte score_intro[] = {
    #embed "../assets/music-intro.pokey"
};

static const byte score_loop[] = {
    #embed "../assets/music-loop.pokey"
};

#ifdef MUSIC_REGION
#pragma data(data)
#endif

static __zeropage volatile word score_ptr;
static volatile byte score_left;
static volatile byte score_phase;
static volatile byte video_rate;

__asm music_tick
{
    lda score_phase
    clc
    adc #50
    cmp video_rate
    bcs playing
    sta score_phase
    rts
playing:
    sbc video_rate
    sta score_phase
    lda score_left
    beq event
    dec score_left
    rts
event:
    ldy #0
    lda (score_ptr),y
    bne load
    lda #<score_loop
    sta score_ptr
    lda #>score_loop
    sta score_ptr + 1
    lda (score_ptr),y
load:
    tax
    dex
    stx score_left
    ldx #0
next_voice:
    iny
    lda (score_ptr),y
    sta $d200,x
    inx
    cpx #6
    bne next_voice
    lda #7
    clc
    adc score_ptr
    sta score_ptr
    bcc done
    inc score_ptr + 1
done:
    rts
}

__asm music_vbi
{
    jsr music_tick
    jmp $e462
}

static void music_start(void)
{
    // GTIA PAL has bits 1-3 clear.
    video_rate = (REG8(0xd014) & 14) ? 60 : 50;
    REG8(0xd208) = 1;
    for (byte i = 0; i < 4; i++)
        REG8(0xd201 + 2 * i) = 0;
    score_ptr = (word)score_intro;
    score_left = 0;
    score_phase = 0;
}

#endif

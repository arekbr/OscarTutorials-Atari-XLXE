#ifndef TUTORIAL_INTERRUPT_H
#define TUTORIAL_INTERRUPT_H

#include "hw.h"

typedef void (*Interrupt)(void);

static void install_dli(Interrupt routine)
{
    NMIEN = 0x40;
    REG16(0x0200) = (word)routine;
    NMIEN = 0xc0;
}

// SETVBV installs an immediate or deferred OS vertical blank vector.
static void install_vbi(byte which, Interrupt routine)
{
    __asm
    {
        lda which
        ldx routine + 1
        ldy routine
        jsr $e45c
    }
}

#endif

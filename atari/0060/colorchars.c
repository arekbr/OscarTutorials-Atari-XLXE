#include "../common/display.h"
#include "../common/interrupt.h"

// Mode 4 uses two-bit character pixels. Convert the ROM font into
// 01 (ink) and 00 (transparent), at four pixels per glyph.
static const char labels[] = "BLACK WHITE RED   GREEN YELLOW";
static const byte colors[] = { 0x00, 0x0e, 0x38, 0xc8, 0x1e, 0x00 };
static volatile byte band;

__asm color_line
{
    pha
    txa
    pha
    ldx band
    lda colors,x
    sta $d40a
    sta $d016
    inx
    cpx #6
    bne save
    ldx #0
save:
    stx band
    pla
    tax
    pla
    rti
}

int main(void)
{
    clear_dma(TEXT, 960);
    clear_dma(FONT, 1024);
    for (byte i = 'A'; i <= 'Z'; i++)
    {
        byte code = screen_code(i);
        for (byte y = 0; y < 8; y++)
        {
            byte bits = ROM_FONT[8 * code + y];
            byte result = 0;
            for (byte x = 0; x < 4; x++)
            {
                result <<= 2;
                if (bits & 0xc0)
                    result |= 1;
                bits <<= 2;
            }
            FONT[8 * code + y] = result;
        }
    }

    text_display(4, true);
    display_off();
    COLOR(4) = 0x06;
    list_begin();
    DL[2] |= 0x80;
    for (byte i = 0; i < 5; i++)
    {
        for (byte j = 0; j < 6; j++)
            TEXT[40 * i + 12 + j] = screen_code(labels[i * 6 + j]);
        list_line(0x84, TEXT + 40 * i);
    }
    list_end();
    display_on(0x22);
    wait_blank();
    install_dli(color_line);
    for (;;) {}
}

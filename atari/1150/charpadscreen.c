#include "../common/display.h"

static const byte ladders_font[] = {
    #embed ctm_chars "../../Resources/ladders.ctm"
};
static const byte ladders_map[] = {
    #embed ctm_map8 "../../Resources/ladders.ctm"
};
static const byte ladders_attributes[] = {
    #embed ctm_attr1 "../../Resources/ladders.ctm"
};

int main(void)
{
    clear_dma(FONT, 1024);
    for (word i = 0; i < sizeof(ladders_font); i++)
    {
        byte bits = ladders_font[i];
        byte pixels = 0;
        byte ink = (ladders_attributes[i >> 3] & 15) == 1 ? 2 : 3;
        for (byte x = 0; x < 4; x++)
        {
            pixels <<= 2;
            if (bits & 0xc0)
                pixels |= ink;
            bits <<= 2;
        }
        FONT[i] = pixels;
    }
    copy_dma(TEXT, ladders_map, 960);
    text_display(4, true);
    for (;;) {}
}

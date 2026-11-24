#include <oscar.h>
#include "../common/display.h"

static const char MilitaryFont[] = {
    #embed ctm_chars lzo "../../Resources/militaryfont.ctm"
};

int main(void)
{
    clear_dma(FONT, 1024);
    oscar_expand_lzo((char *)FONT, MilitaryFont);
    for (word i = 0; i < 960; i++)
        TEXT[i] = i & 63;
    text_display(2, true);
    for (;;) {}
}

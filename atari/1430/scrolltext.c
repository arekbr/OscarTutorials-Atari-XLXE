#include "../common/scroll.h"

static const char text[] =
	"LABORUM RERUM QUO. QUASI IN, SEQUI, TENETUR VOLUPTATEM RERUM "
	"PORRO NON ET MAIORES ALIAS ODIO EST EOS. MAGNAM APERIAM CUM ET "
	"ESSE TEMPORE ITAQUE TEMPORA VOLUPTAS ET IPSAM IPSAM EARUM. ID "
	"SUSCIPIT QUIA RERUM REPREHENDERIT ERROR ET UT. DOLOR ID "
	"CORPORIS, EOS? UNDE VERO ISTE QUIA? EAQUE EAQUE. IN. AUT ID "
	"EXPEDITA ILLUM MOLESTIAS, ";

static void load_row(word start)
{
    for (byte x = 0; x < SCROLL_PITCH; x++)
    {
        scroll_back[x] = screen_code(text[start++]);
        if (start == sizeof(text) - 1)
            start = 0;
    }
}

static volatile byte *make_list(const volatile byte *row)
{
    volatile byte *list = visible_list == DL ? ALT_DL : DL;
    volatile byte *p = list;
    *p++ = 0x70;
    *p++ = 0x70;
    *p++ = 0x70;
    *p++ = 0x42;
    *p++ = 0x00;
    *p++ = 0x80;
    for (byte y = 1; y < 23; y++)
        *p++ = 2;
    *p++ = 0x52;
    *p++ = (byte)(word)row;
    *p++ = (word)row >> 8;
    *p++ = 0x41;
    *p++ = (byte)(word)list;
    *p = (word)list >> 8;
    return list;
}

int main(void)
{
    clear_dma(TEXT, 960);
    text_display(2, false);
    for (word x = 0;;)
    {
        load_row(x);
        volatile byte *next = make_list(scroll_back);
        publish_scroll(next, 12, 0);
        scroll_flip();
        for (byte fine = 1; fine < 4; fine++)
        {
            wait_blank();
            HSCROL = 12 - fine;
        }
        if (++x == sizeof(text) - 1)
            x = 0;
    }
}

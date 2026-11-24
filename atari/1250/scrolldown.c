#include "../common/scroll.h"

static void prepare_row(volatile byte *screen)
{
    for (signed char y = 24; y > 0; y--)
        for (byte x = 0; x < 40; x++)
            screen[40 * y + x] = scroll_front[40 * (y - 1) + x];
    for (byte x = 0; x < 40; x++)
        screen[x] = diagonal();
}

int main(void)
{
    for (word i = 0; i < 1000; i++)
        scroll_front[i] = diagonal();

    text_display(2, false);
    volatile byte *next = prepare_scroll(scroll_front, 2, 40, false, true);
    publish_scroll(next, 0, 7);

    for (;;)
    {
        prepare_row(scroll_back);
        next = prepare_scroll(scroll_back, 2, 40, false, true);
        for (byte fine = 1; fine < 8; fine++)
        {
            wait_blank();
            VSCROL = 7 - fine;
        }
        publish_scroll(next, 0, 7);
        scroll_flip();
    }
}

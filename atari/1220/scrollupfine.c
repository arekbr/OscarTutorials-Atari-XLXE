#include "../common/scroll.h"

static void prepare_row(volatile byte *screen)
{
    for (word i = 0; i < 960; i++)
        screen[i] = scroll_front[i + 40];
    for (byte x = 0; x < 40; x++)
        screen[960 + x] = diagonal();
}

int main(void)
{
    for (word i = 0; i < 1000; i++)
        scroll_front[i] = diagonal();

    text_display(2, false);
    volatile byte *next = prepare_scroll(scroll_front, 2, 40, false, true);
    publish_scroll(next, 0, 0);

    for (;;)
    {
        prepare_row(scroll_back);
        next = prepare_scroll(scroll_back, 2, 40, false, true);
        for (byte fine = 1; fine < 8; fine++)
        {
            wait_blank();
            VSCROL = fine;
        }
        publish_scroll(next, 0, 0);
        scroll_flip();
    }
}

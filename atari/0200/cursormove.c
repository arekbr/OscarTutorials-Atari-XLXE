#include "../common/display.h"

int main(void)
{
    byte x = 0, y = 0;
    clear_dma(TEXT, 960);
    text_display(2, false);

    for (;;)
    {
        TEXT[40 * y + x] = 0x80;
        wait_frame();
        byte key = KEYCODE;
        KEYCODE = 255;
        TEXT[40 * y + x] = 0;
        // OS key codes for Ctrl+arrow, and joystick 1.
        if ((key == 0x86 || joy_x() < 0) && x)
            x--;
        if ((key == 0x87 || joy_x() > 0) && x < 39)
            x++;
        if ((key == 0x8e || joy_y() < 0) && y)
            y--;
        if ((key == 0x8f || joy_y() > 0) && y < 23)
            y++;
    }
}

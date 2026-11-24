#include "../common/display.h"

#pragma section(music, 0)
#pragma region(music, 0x7400, 0x8000, , , {music})
#define MUSIC_REGION
#include "../common/music.h"

#pragma section(font, 0)
#pragma region(font, 0x8800, 0x8c00, , , {font})
#pragma data(font)
static const byte library_font[] = {
    #embed "../../Resources/militaryfont.bin"
};
#pragma data(data)

int main(void)
{
    for (word i = 0; i < 960; i++)
        TEXT[i] = i % (sizeof(library_font) / 8);
    text_display(2, true);
    // Mark the font as used for the linker and select its page in ANTIC.
    CHBAS = (word)library_font >> 8;
    music_start();
    install_vbi(7, music_vbi);
    for (;;) {}
}

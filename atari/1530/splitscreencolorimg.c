#include "../common/photo.h"

static const char message[] =
    "LABORUM RERUM QUO. QUASI IN, SEQUI, TENETUR VOLUPTATEM RERUM "
    "PORRO NON ET MAIORES ALIAS ODIO EST EOS. MAGNAM APERIAM CUM ET "
    "ESSE TEMPORE ITAQUE TEMPORA VOLUPTAS ET IPSAM IPSAM EARUM. ID "
    "SUSCIPIT QUIA RERUM REPREHENDERIT ERROR ET UT. DOLOR ID "
    "CORPORIS, EOS? UNDE VERO ISTE QUIA? EAQUE EAQUE. IN. AUT ID "
    "EXPEDITA ILLUM MOLESTIAS, ";

int main(void)
{
    // The text strip has its own LMS and HSCROL setting.
    clear_dma(TEXT, 48);
    photo_start(176, true);
    byte fine = 0;
    word start = 0;
    for (;;)
    {
        wait_blank();
        HSCROL = 12 - fine;
        if (!fine)
        {
            word src = start;
            for (byte x = 0; x < 48; x++)
            {
                TEXT[x] = screen_code(message[src++]);
                if (src == sizeof(message) - 1) src = 0;
            }
            if (++start == sizeof(message) - 1) start = 0;
        }
        fine = (fine + 1) & 3;
    }
}

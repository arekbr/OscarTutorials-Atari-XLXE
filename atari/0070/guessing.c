#include "../common/console.h"

int main(void)
{
    char answer[8];
    do
    {
        byte random;
        do
        {
            random = REG8(0xd20a);
        } while (random >= 252);
        byte dice = random % 6 + 1;
        do
        {
            printf("\nInput your guess from 1 to 6: ");
            read_line(answer, sizeof(answer));
        } while (answer[0] < '1' || answer[0] > '6' || answer[1]);

        if (answer[0] - '0' == dice)
            printf("You guessed right!\n");
        else
            printf("Wrong number, the dice rolled %d\n", dice);

        do
        {
            printf("Try again (y/n): ");
            read_line(answer, sizeof(answer));
        } while (answer[1] || (answer[0] != 'y' && answer[0] != 'Y' &&
                              answer[0] != 'n' && answer[0] != 'N'));
    } while (answer[0] == 'y' || answer[0] == 'Y');
    return 0;
}

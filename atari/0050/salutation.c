#include "../common/console.h"

int main(void)
{
    char name[41];
    printf("Please enter your name:\n");
    read_line(name, sizeof(name));
    printf("\nHello %s !\n", name);
    return 0;
}

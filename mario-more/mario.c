#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height <= 0);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < height - 1 - i; j++)
        {
            printf(" ");
        }
        for(int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("  ");
        for(int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}

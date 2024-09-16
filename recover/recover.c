#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    uint8_t buffer[512];
    int n = 0;
    FILE *img;
    bool isFoundJPG = false;

    while (fread(buffer, 1, 512, card) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            char fileName[8];
            sprintf(fileName, "%03i.jpg", n);
            if (n == 0)
            {
                img = fopen(fileName, "w");
                fwrite(buffer, 1, 512, img);
            }
            else
            {
                fclose(img);
                img = fopen(fileName, "w");
                fwrite(buffer, 1, 512, img);
            }
            n++;
            isFoundJPG = true;
        }
        else
        {
            if (isFoundJPG)
            {
                fwrite(buffer, 1, 512, img);
            }
        }
    }

    fclose(img);
    fclose(card);
}

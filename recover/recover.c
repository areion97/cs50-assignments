#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    FILE *fileOut = NULL;
    char filename[8];

    if (file == NULL)
    {
        return 1;
    }

    BYTE buffer[512];
    int counter = 0;
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >= 0xe && buffer[3] <= 0xef))
        {
            if (fileOut != NULL)
            {
                fclose(fileOut);
            }
            sprintf(filename, "%03i.jpg", counter);
            counter++;
            fileOut = fopen(filename, "w");
        }
        if (fileOut != NULL)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, fileOut);
        }
    }
    fclose(fileOut);
    fclose(file);

}